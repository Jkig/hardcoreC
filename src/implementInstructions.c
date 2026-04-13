#include "processor.h"
#include "implementInstructions.h"
#include "ISADef.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>


// External globals
extern uint64_t program_state;
extern uint64_t program_return;
extern Registers registers;
extern volatile uint64_t interrupt_signals;    // bit field
extern uint8_t ram[RAM_SIZE];



////////////////////////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////////////////////////

uint64_t *get_register_ptr(uint8_t reg_id) {
    if (reg_id < 1) {
        printf("%s, Failure, register ID of 0?\n", __func__);
        return NULL;
    } else if (reg_id > GENERAL_PURPOSE_REGISTER_COUNT + LAST_REGISTER_BEFORE_GP) {
        printf("%s, Failure, register ID of 0?\n", __func__);
        return NULL;
    }

    switch (reg_id) {
        case REG_PC:        return &registers.pc;
        case REG_SP:        return &registers.sp;
        case REG_RES:       return &registers.res;
        case REG_STATUS:    return &registers.status;
        default:            return &registers.gp[reg_id - GP_REGISTERS_OFFSET];
    }
}


int get_register_id(uint64_t *reg_ptr) {
    if (reg_ptr == &registers.pc)     return REG_PC;
    if (reg_ptr == &registers.sp)     return REG_SP;
    if (reg_ptr == &registers.res)    return REG_RES;
    if (reg_ptr == &registers.status) return REG_STATUS;

    if (reg_ptr >= &registers.gp[0] &&
        reg_ptr <  &registers.gp[GENERAL_PURPOSE_REGISTER_COUNT]) {
        return GP_REGISTERS_OFFSET + (int)(reg_ptr - &registers.gp[0]);
    }

    return -1;
}

#define FETCH_3REGS(dst, srcA, srcB, val)                                \
    printf("%s: exec\n", __func__);                                      \
    InstructionBits raw_instruction = {0};                               \
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction)); \
    uint64_t *dst  = get_register_ptr(raw_instruction.ins.regA);         \
    uint64_t *srcA = get_register_ptr(raw_instruction.ins.regB);         \
    uint64_t *srcB = get_register_ptr(raw_instruction.ins.regC);         \
    uint32_t val = raw_instruction.ins.val;                              \
    registers.pc += sizeof(Instruction);                                 \
    if (!(dst) || !(srcA) || !(srcB)) {                                  \
        printf("%s, Failure, NULL register\n", __func__);                \
        return;                                                          \
    }

#define FETCH_2REGS(dst, src, val)                                       \
    printf("%s: exec\n", __func__);                                      \
    InstructionBits raw_instruction = {0};                               \
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction)); \
    uint64_t *dst  = get_register_ptr(raw_instruction.ins.regA);         \
    uint64_t *src = get_register_ptr(raw_instruction.ins.regB);          \
    uint32_t val = raw_instruction.ins.val;                              \
    registers.pc += sizeof(Instruction);                                 \
    if (!(dst) || !(src)) {                                              \
        printf("%s, Failure, NULL register\n", __func__);                \
        return;                                                          \
    }

////////////////////////////////////////////////////////////////////////////////
// Operations
////////////////////////////////////////////////////////////////////////////////


void op_INVALID() {

};


void op_NOOP() {
    registers.pc += sizeof(Instruction);
}

// TODO: op_TRUE_NOOP_SPIN -> needs interrupt to escape, same as op_INVALID, empty

void op_MOV_REG() {
    printf("%s: exec\n", __func__);
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint64_t *src = get_register_ptr(raw_instruction.ins.regB);
    registers.pc += sizeof(Instruction);

    if (!dst || !src) {
        printf("%s, Failure, NULL register\n", __func__);
        return;
    }

    *dst = *src;
}

void op_MOV_IMM_TO_LO() {
    printf("%s: exec\n", __func__);
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint32_t val = raw_instruction.ins.val;
    registers.pc += sizeof(Instruction);

    if (!dst) {
        printf("%s, Failure, NULL register\n", __func__);
        return;
    }

    *dst &= 0xFFFFFFFF00000000;
    *dst |= val;
}

void op_MOV_IMM_TO_HI() {
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint32_t val = raw_instruction.ins.val;
    registers.pc += sizeof(Instruction);

    if (!dst) {
        printf("%s, Failure, NULL register\n", __func__);
        return;
    }

    *dst &= 0x00000000FFFFFFFF;
    *dst |= val;
}

void op_ADD_SRC_SRC() {
    printf("%s: exec\n", __func__);
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA + *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_ADD_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src + val;
    // TODO: Update Status register (beautiful side effects)
}


void op_SUB_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA - *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_SUB_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src - val;
    // TODO: Update Status register (beautiful side effects)
}

void op_MUL_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA * *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_MUL_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src * val;
    // TODO: Update Status register (beautiful side effects)
}

void op_DIV_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    if (*srcB == 0) {
        printf("divide by zero!!");
        *dst = 0;
    } else {
        *dst = *srcA / *srcB;
    }
    // TODO: Update Status register (beautiful side effects)
}

void op_DIV_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    if (val == 0) {
        printf("divide by zero!!");
        *dst = 0;
    } else {
        *dst = *src / val;
    }
    // TODO: Update Status register (beautiful side effects)
}

void op_MOD_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    if (*srcB == 0) {
        printf("divide by zero!!");
        *dst = 0;
    } else {
        *dst = *srcA % *srcB;
    }
    // TODO: Update Status register (beautiful side effects)
}

void op_MOD_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    if (val == 0) {
        printf("divide by zero!!");
        *dst = 0;
    } else {
        *dst = *src % val;
    }
    // TODO: Update Status register (beautiful side effects)
}

void op_LSH_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA << *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_LSH_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src << val;
    // TODO: Update Status register (beautiful side effects)
}

void op_RSH_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA >> *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_RSH_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src >> val;
    // TODO: Update Status register (beautiful side effects)
}

void op_AND_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA & *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_AND_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src & val;
    // TODO: Update Status register (beautiful side effects)
}

void op_OR_SRC_SRC() {
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint64_t *srcA = get_register_ptr(raw_instruction.ins.regB);
    uint64_t *srcB = get_register_ptr(raw_instruction.ins.regC);
    registers.pc += sizeof(Instruction);

    if (!dst || !srcA  || !srcB) {
        printf("%s, Failure, NULL register\n", __func__);
        return;
    }

    *dst = *srcA | *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_OR_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src | val;
    // TODO: Update Status register (beautiful side effects)
}

void op_XOR_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA ^ *srcB;
    // TODO: Update Status register (beautiful side effects)
}

void op_XOR_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src ^ val;
    // TODO: Update Status register (beautiful side effects)
}

void op_JMP() {
    registers.pc += sizeof(Instruction);
}

void op_LOAD() {
    registers.pc += sizeof(Instruction);
}

void op_STORE() {
    registers.pc += sizeof(Instruction);
}

void op_CMP() {
    registers.pc += sizeof(Instruction);
}

void op_SW_INTERUPT() {
    printf("%s: exec\n", __func__);
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    registers.pc += sizeof(Instruction);

    /// interrupt_no is a register holding the value for the interrupt to trigger from software
    uint64_t *interrupt_no = get_register_ptr(raw_instruction.ins.regA);

    if (!interrupt_no) {
        printf("%s, Failure, NULL register\n", __func__);
        return;
    }

    if (*interrupt_no < SOFTWARE_INTERRUPT_HI_0 || SOFTWARE_INTERRUPT_LOW_3 < *interrupt_no) {
        printf("%s, This is an invalid interrupt, can't trigger %llu from software\n", __func__, *interrupt_no);
        return;
    }

    interrupt_signals |= (1 << *interrupt_no);
}

// Globals
instruction instruction_table[ISA_COUNT] = {
    // Just copy in from Opcode definition, and swap place by place with the op_INSTRUCTION_var
    op_INVALID,
    op_NOOP,
    op_LOAD,
    op_STORE,
    op_JMP,
    op_CMP,

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
    op_ADD_SRC_IMM,
    op_OR_SRC_SRC,
    op_OR_SRC_IMM,
    op_XOR_SRC_SRC,
    op_XOR_SRC_IMM,
};
