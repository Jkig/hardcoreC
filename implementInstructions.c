#include "processor.h"
#include "implementInstructions.h"
#include "ISADef.h"
#include <stdint.h>
#include <stddef.h>


// External globals
extern uint64_t program_state;
extern uint64_t program_return = 0;
extern Registers registers;
extern uint8_t ram[RAM_SIZE];


// Globals
instruction instruction_table[ISA_COUNT] = {
    // Just copy in from Opcode definition, and swap place by place with the op_INSTRUCTION_var
};


////////////////////////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////////////////////////

uint64_t *get_register_ptr(uint8_t reg_id) {
    if (reg_id < 1) {
        printf("%s, Failure, register ID of 0?", __func__);
        return NULL;
    } else if (reg_id > GENERAL_PURPOSE_REGISTER_COUNT + LAST_REGISTER_BEFORE_GP) {
        printf("%s, Failure, register ID of 0?", __func__);
        return NULL;
    }

    switch (reg_id) {
        case REG_PC:        return &registers.pc;
        case REG_SP:        return &registers.sp;
        case REG_RES:       return &registers.res;
        case REG_STATUS:    return &registers.status;
        default:            return &registers.gp[reg_id - GP_REGISTER_OFFSET];
    }
}


int get_register_id(uint64_t *reg_ptr) {
    if (reg_ptr == &registers.pc)     return REG_PC;
    if (reg_ptr == &registers.sp)     return REG_SP;
    if (reg_ptr == &registers.res)    return REG_RES;
    if (reg_ptr == &registers.status) return REG_STATUS;

    if (reg_ptr >= &registers.gp[0] &&
        reg_ptr <  &registers.gp[GENERAL_PURPOSE_REGISTER_COUNT]) {
        return GP_REGISTER_OFFSET + (int)(reg_ptr - &registers.gp[0]);
    }

    return -1;
}

#define FETCH_3REGS(dst, srcA, srcB, val)                                 \
    InstructionBits raw_instruction = {0};                               \
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction)); \
    uint64_t *dst  = get_register_ptr(raw_instruction.ins.regA);         \
    uint64_t *srcA = get_register_ptr(raw_instruction.ins.regB);         \
    uint64_t *srcB = get_register_ptr(raw_instruction.ins.regC);         \
    uint32_t val = raw_instruction.ins.val;                              \
    if (!(dst) || !(srcA) || !(srcB)) {                                  \
        printf("%s, Failure, NULL register\n", __func__);                \
        registers.pc += sizeof(Instruction);                             \
        return;                                                          \
    }

#define FETCH_2REGS(dst, src, val)                                       \
    InstructionBits raw_instruction = {0};                               \
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction)); \
    uint64_t *dst  = get_register_ptr(raw_instruction.ins.regA);         \
    uint64_t *src = get_register_ptr(raw_instruction.ins.regB);          \
    uint32_t val = raw_instruction.ins.val;                              \
    if (!(dst) || !(src)) {                                              \
        printf("%s, Failure, NULL register\n", __func__);                \
        registers.pc += sizeof(Instruction);                             \
        return;                                                          \
    }

////////////////////////////////////////////////////////////////////////////////
// Operations
////////////////////////////////////////////////////////////////////////////////

void op_NOOP() {
    registers.pc += sizeof(Instruction);
}

void op_MOV_REG() {
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint64_t *src = get_register_ptr(raw_instruction.ins.regB);

    if (!dst || !src) {
        printf("%s, Failure, NULL register ", __func__);
        registers.pc += sizeof(Instruction);
        return;
    }

    *dst = *src;
    registers.pc += sizeof(Instruction);
}

void op_MOV_IMM_TO_LO() {
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint32_t val = raw_instruction.ins.val;

    if (!dst) {
        printf("%s, Failure, NULL register ", __func__);
        registers.pc += sizeof(Instruction);
        return;
    }

    *dst &= 0xFFFFFFFF00000000;
    *dst |= val;
    registers.pc += sizeof(Instruction);
}

void op_MOV_IMM_TO_HI() {
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint32_t val = raw_instruction.ins.val;

    if (!dst) {
        printf("%s, Failure, NULL register ", __func__);
        registers.pc += sizeof(Instruction);
        return;
    }

    *dst &= 0x00000000FFFFFFFF;
    *dst |= val;
    registers.pc += sizeof(Instruction);
}

void op_ADD_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA + *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_ADD_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src + val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}


void op_SUB_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA - *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_SUB_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src - val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_MUL_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA * *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_MUL_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src * val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
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
    registers.pc += sizeof(Instruction);
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
    registers.pc += sizeof(Instruction);
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
    registers.pc += sizeof(Instruction);
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
    registers.pc += sizeof(Instruction);
}

void op_LSH_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA << *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_LSH_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src << val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_RSH_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA >> *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_RSH_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src >> val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_AND_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA & *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_AND_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src & val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_OR_SRC_SRC() {
    InstructionBits raw_instruction = {0};
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    uint64_t *dst = get_register_ptr(raw_instruction.ins.regA);
    uint64_t *srcA = get_register_ptr(raw_instruction.ins.regB);
    uint64_t *srcB = get_register_ptr(raw_instruction.ins.regC);

    if (!dst || !srcA  || !srcB) {
        printf("%s, Failure, NULL register ", __func__);
        registers.pc += sizeof(Instruction);
        return;
    }

    *dst = *srcA | *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_OR_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src | val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_XOR_SRC_SRC() {
    FETCH_3REGS(dst, srcA, srcB, val);

    *dst = *srcA ^ *srcB;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void op_XOR_SRC_IMM() {
    FETCH_2REGS(dst, src, val);

    *dst = *src ^ val;
    // TODO: Update Status register (beautiful side effects)
    registers.pc += sizeof(Instruction);
}

void JMP() {
    *pc = addr;
}


/* Notes
 * They all just operate on the global register set, but some do have info with code, ex add r0 r1 #5

 * How do we normally do this, having the instruction take a value or an address, or a register?
 *   start with load and store, these are easy in my model
 * I should also implement a software interrupt instruction to go here! its not too hard
 * I'll sprintf into a buffer, then use a single instruction pointing, this can be a bit special
 */

