#include "processor.h"
#include "ISADef.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>




// External globals
extern uint64_t program_state;
extern uint64_t program_return;
extern CPU cpu;
extern volatile uint64_t interrupt_signals;  // bit field
extern uint8_t ram[RAM_SIZE_BYTES];


extern bool print_execute_instruction;


////////////////////////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////////////////////////

void invalid_instruction() {
  // TODO: how should I fail, one msg and a function call, swap this in,, I
  // think update program state
}

uint64_t *get_register_ptr(uint8_t reg_id) {
  if (reg_id < 1) {
    printf("%s, Failure, register ID of 0?\n", __func__);
    return NULL;
  } else if (reg_id >
      GENERAL_PURPOSE_REGISTER_COUNT + LAST_REGISTER_BEFORE_GP) {
    printf("%s, Failure, register ID too high?\n", __func__);
    return NULL;
  }

  switch (reg_id) {
    case REG_PC:      return &registers.pc;
    case REG_SP:      return &registers.sp;
    case REG_STATUS:  return &registers.status;
    default:          return &registers.gp[reg_id - GP_REGISTERS_OFFSET];
  }
}


int get_register_id(uint64_t *reg_ptr) {
  if (reg_ptr == &registers.pc)   return REG_PC;
  if (reg_ptr == &registers.sp)   return REG_SP;
  if (reg_ptr == &registers.status) return REG_STATUS;

  if (reg_ptr >= &registers.gp[0] &&
      reg_ptr <  &registers.gp[GENERAL_PURPOSE_REGISTER_COUNT]) {
    return GP_REGISTERS_OFFSET + (int)(reg_ptr - &registers.gp[0]);
  }

  return -1;
}

// TODO: out of range in the other way too
#define FETCH_3REGS(dst, srcA, srcB, val)                                \
if (print_execute_instruction) printf("%s: exec\n", __func__);           \
InstructionBits raw_instruction = {0};                                   \
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction)); \
  uint64_t *dst  = get_register_ptr(raw_instruction.ins.regA);           \
  uint64_t *srcA = get_register_ptr(raw_instruction.ins.regB);           \
  uint64_t *srcB = get_register_ptr(raw_instruction.ins.regC);           \
  uint32_t val = raw_instruction.ins.val;                                \
  registers.pc += sizeof(Instruction);                                   \
  if (!(dst) || !(srcA) || !(srcB)) {                                    \
    printf("%s, Failure, NULL register\n", __func__);                    \
    invalid_instruction();                                               \
    return;                                                              \
  }

#define FETCH_2REGS(dst, src, val)                                       \
  if (print_execute_instruction) printf("%s: exec\n", __func__);         \
  InstructionBits raw_instruction = {0};                                 \
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction)); \
  uint64_t *dst  = get_register_ptr(raw_instruction.ins.regA);           \
  uint64_t *src  = get_register_ptr(raw_instruction.ins.regB);           \
  uint32_t val = raw_instruction.ins.val;                                \
  registers.pc += sizeof(Instruction);                                   \
  if (!(dst) || !(src)) {                                                \
    printf("%s, Failure, NULL register\n", __func__);                    \
    invalid_instruction();                                               \
    return;                                                              \
  }

////////////////////////////////////////////////////////////////////////////////
// Operations
////////////////////////////////////////////////////////////////////////////////


void op_INVALID() {
  invalid_instruction();
};


void op_NOOP() {
  registers.pc += sizeof(Instruction);
}

// TODO: op_TRUE_NOOP_SPIN -> needs interrupt to escape, same as op_INVALID,
// empty

void op_MOV_REG() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
  uint64_t *src = get_register_ptr(raw_instruction.ins.regB);
  registers.pc += sizeof(Instruction);

  if (!dst || !src) {
    printf("%s, Failure, NULL register\n", __func__);
    invalid_instruction();
    return;
  }

  *dst = *src;
}

void op_MOV_IMM_TO_LO() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
  uint32_t val = raw_instruction.ins.val;
  registers.pc += sizeof(Instruction);

  if (!dst) {
    printf("%s, Failure, NULL register\n", __func__);
    invalid_instruction();
    return;
  }

  *dst &= 0xFFFFFFFF00000000;
  *dst |= val;
}

void op_MOV_IMM_TO_HI() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
  uint32_t val = raw_instruction.ins.val;
  registers.pc += sizeof(Instruction);

  if (!dst) {
    printf("%s, Failure, NULL register\n", __func__);
    invalid_instruction();
    return;
  }

  *dst &= 0x00000000FFFFFFFF;
  *dst |= val;
}

void op_ADD_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA + *srcB;
  // TODO: Update Status register (beautiful side effects) - Ex: overflow
}

void op_ADD_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src + val;
  // TODO: Update Status register (beautiful side effects) - Ex: overflow
}


void op_SUB_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA - *srcB;
}

void op_SUB_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src - val;
}

void op_MUL_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA * *srcB;
  // TODO: Update Status register (beautiful side effects) - Ex: overflow
}

void op_MUL_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src * val;
  // TODO: Update Status register (beautiful side effects) - Ex: overflow
}

void op_DIV_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  if (*srcB == 0) {
    printf("divide by zero!!");
    // invalid_instruction();// TODO: do I care about this, just a status
    // update?
    // TODO: Update Status register (beautiful side effects)
    *dst = 0;
  } else {
    *dst = *srcA / *srcB;
  }
}

void op_DIV_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  if (val == 0) {
    printf("divide by zero!!");
    // invalid_instruction();// TODO: do I care about this, just a status
    // update?
    // TODO: Update Status register (beautiful side effects)
    *dst = 0;
  } else {
    *dst = *src / val;
  }
}

void op_MOD_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  if (*srcB == 0) {
    printf("divide by zero!!");
    invalid_instruction();
    // TODO: Update Status register (beautiful side effects)
    *dst = 0;
  } else {
    *dst = *srcA % *srcB;
  }
}

void op_MOD_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  if (val == 0) {
    printf("divide by zero!!");
    // invalid_instruction();// TODO: do I care about this, just a status
    // update?
    // TODO: Update Status register (beautiful side effects)
    *dst = 0;
  } else {
    *dst = *src % val;
  }
}

void op_LSH_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA << *srcB;
}

void op_LSH_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src << val;
}

void op_RSH_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA >> *srcB;
  // TODO: Update Status register (beautiful side effects) - Ex: overflow
}

void op_RSH_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src >> val;
  // TODO: Update Status register (beautiful side effects) - Ex: overflow
}

void op_AND_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA & *srcB;
}

void op_AND_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src & val;
}

void op_OR_SRC_SRC() {
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
  uint64_t *srcA = get_register_ptr(raw_instruction.ins.regB);
  uint64_t *srcB = get_register_ptr(raw_instruction.ins.regC);
  registers.pc += sizeof(Instruction);

  if (!dst || !srcA || !srcB) {
    printf("%s, Failure, NULL register\n", __func__);
    invalid_instruction();
    return;
  }

  *dst = *srcA | *srcB;
}

void op_OR_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src | val;
}

void op_XOR_SRC_SRC() {
  FETCH_3REGS(dst, srcA, srcB, val);

  *dst = *srcA ^ *srcB;
}

void op_XOR_SRC_IMM() {
  FETCH_2REGS(dst, src, val);

  *dst = *src ^ val;
}

// I know that this *should* be a bit different, but I'm making assembler
// simpler, so just load the value to PC
void op_B() {
  // TODO: Should I general branch be able to b to some register? I know its
  // already fairly limited
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint32_t val = raw_instruction.ins.val;

  if (!val) {
    printf("%s, Failure, NULL val, I'm not stopping it, but proc should "
        "fail here!!\n",
        __func__);
    registers.pc = 0;// So its easy to see problem still
    invalid_instruction();
    return;
  }
  // The value is assumed to always fit in the low 32 bits, this is a fine
  // assumption
  registers.pc = val;
}

void op_BEQ() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint32_t val = raw_instruction.ins.val;

  if (!val) {
    printf("%s, Failure, NULL val, I'm not stopping it, but proc should "
        "fail here!!\n",
        __func__);
    invalid_instruction();
    return;
  }

  if (registers.status && EQUAL) {
    // The value is assumed to always fit in the low 32 bits, this is a
    // fine assumption
    registers.pc = val;
  } else {
    registers.pc += sizeof(Instruction);
  }
}

void op_BNEQ() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint32_t val = raw_instruction.ins.val;

  if (!val) {
    printf("%s, Failure, NULL val, I'm not stopping it, but proc should "
        "fail here!!\n",
        __func__);
    invalid_instruction();
    return;
  }

  if (!(registers.status && EQUAL)) {
    // The value is assumed to always fit in the low 32 bits, this is a
    // fine assumption
    registers.pc = val;
  } else {
    registers.pc += sizeof(Instruction);
  }
}

void op_LOAD() {
  // Usage: load (into register A), (the value from the address B), (with
  // the size in bytes)
  // Encoding the bytes size as val
  FETCH_2REGS(dst, src, val);

  if (val != 1 || val != 2 || val != 4 || val != 8) {
    printf("%s, Failure, invalid size/ instruction, trying to load "
        "something that's wrong",
        __func__);
    invalid_instruction();
    return;
  }

  uint64_t pointer_to_mem_as_index = *src;
  if ((pointer_to_mem_as_index + (val - 1)) >= RAM_SIZE_BYTES) {
    // Shouldn't really ever load 0, but I'm modeling the hardware as
    // here, so its fine
    printf("%s, Memory out of range!", __func__);
    invalid_instruction();
    return;
  }

  for (int8_t i = 0; i < val; i++) {
    // TODO: Make sure my byte order is right
    *dst = 0;
    *dst += (ram[pointer_to_mem_as_index + i] << i);
  }
}

void op_STORE() {
  // Usage: load (into register A), (the value from the address B), (with
  // the size in bytes)
  // Encoding the bytes size as val
  FETCH_2REGS(dst, src, val);

  if (val != 1 || val != 2 || val != 4 || val != 8) {
    printf("%s, Failure, invalid size/ instruction, trying to load "
        "something that's wrong",
        __func__);
    invalid_instruction();
    return;
  }

  uint64_t pointer_to_mem_as_index = *dst;
  if ((pointer_to_mem_as_index + (val - 1)) >= RAM_SIZE_BYTES) {
    // Shouldn't really ever load 0, but I'm modeling the hardware as
    // here, so its fine
    printf("%s, Memory out of range!", __func__);
    invalid_instruction();
    return;
  }

  for (int8_t i = 0; i < val; i++) {
    // TODO: Make sure my byte order is right
    ram[pointer_to_mem_as_index + i] = ((*src && (0xFF << i)) >> i);
  }
}

void op_CMP_IMM() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint64_t *cmp_reg = get_register_ptr(raw_instruction.ins.regA);
  uint32_t val = raw_instruction.ins.val;
  registers.pc += sizeof(Instruction);

  registers.status &= ~(COMPARISON_FLAGS);
  if (*cmp_reg < val) {
    registers.status |= LESS;
  } else if (*cmp_reg > val) {
    registers.status |= GREATER;
  } else if (*cmp_reg == val) {
    registers.status |= EQUAL;
  }
}

void op_CMP_SRC() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  uint64_t *cmp_reg1 = get_register_ptr(raw_instruction.ins.regA);
  uint64_t *cmp_reg2 = get_register_ptr(raw_instruction.ins.regB);
  registers.pc += sizeof(Instruction);

  registers.status &= ~(COMPARISON_FLAGS);
  if (*cmp_reg1 < *cmp_reg2) {
    registers.status |= LESS;
  } else if (*cmp_reg1 > *cmp_reg2) {
    registers.status |= GREATER;
  } else if (*cmp_reg1 == *cmp_reg2) {
    registers.status |= EQUAL;
  }
}

void op_SW_INTERUPT() {
  if (print_execute_instruction) {
    printf("%s: exec\n", __func__); // Not seen in the macro here
  }
  InstructionBits raw_instruction = {0};
  memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
  registers.pc += sizeof(Instruction);

  /// interrupt_no is a register holding the value for the interrupt to
  /// trigger from software
  uint64_t *interrupt_no = get_register_ptr(raw_instruction.ins.regA);

  if (!interrupt_no) {
    printf("%s, Failure, NULL register\n", __func__);
    invalid_instruction();
    return;
  }

  if (*interrupt_no < SOFTWARE_INTERRUPT_HI_0 ||
      SOFTWARE_INTERRUPT_LOW_3 < *interrupt_no) {
    printf("%s, This is an invalid interrupt, can't trigger %lu from "
        "software\n",
        __func__, *interrupt_no);
    invalid_instruction();
    return;
  }

  interrupt_signals |= (1 << *interrupt_no);
}

// Globals
instruction instruction_table[OPCODE_COUNT] = {
  // Just copy in from Opcode definition, and swap place by place with the
  // op_INSTRUCTION_var
  op_INVALID,
  op_NOOP,
  op_LOAD,
  op_STORE,
  op_B,
  op_BEQ,
  op_BNEQ,
  op_CMP_SRC,
  op_CMP_IMM,

  op_MOV_REG,
  op_MOV_IMM_TO_LO,
  op_MOV_IMM_TO_HI,

  op_ADD_SRC_SRC,
  op_ADD_SRC_IMM,
  op_SUB_SRC_SRC,
  op_SUB_SRC_IMM,
  op_MUL_SRC_SRC,
  op_MUL_SRC_IMM,
  op_DIV_SRC_SRC,
  op_DIV_SRC_IMM,
  op_MOD_SRC_SRC,
  op_MOD_SRC_IMM,
  op_LSH_SRC_SRC,
  op_LSH_SRC_IMM,
  op_RSH_SRC_SRC,
  op_RSH_SRC_IMM,
  op_AND_SRC_SRC,
  op_AND_SRC_IMM,
  op_OR_SRC_SRC,
  op_OR_SRC_IMM,
  op_XOR_SRC_SRC,
  op_XOR_SRC_IMM,
};
