// If you are exploring, this is the place to start, this is what you can use to write your .dasm programs
#ifndef DEREKS_ASSEMBLY_H
#define DEREKS_ASSEMBLY_H
#include "Assembler.h"
#include "processor.h"

StringToInt REGISTER_NAMES[] = {
    {"pc",      REG_PC},
    {"sp",      REG_SP},
    {"status",  REG_STATUS},
    {"gp",      GP_REGISTERS_OFFSET},
};

StringToInt direct_opcodes[] = {
    {"noop",    NOOP},
    {"load",    LOAD},
    {"store",   STORE},
    {"b",       B},
    {"beq",     BEQ},
    {"bneq",    BNEQ},// TODO: I think I want to get rid of this, I'm keeping it streamlined
};

StringToInt mov[] = {
    {"mov",     MOV_REG},
};

StringToInt cmp[] = {
    {"cmp",     CMP_SRC},
};

// All either src or IMM, based on if the third param is a number
// src is first, then if it is imm add 1
StringToInt arithmetic_instruction_to_opcode[] = {
    {"add",     ADD_SRC_SRC},
    {"sub",     SUB_SRC_SRC},
    {"mul",     MUL_SRC_SRC},
    {"div",     DIV_SRC_SRC},
    {"mod",     MOD_SRC_SRC},
    {"lsh",     LSH_SRC_SRC},
    {"rsh",     RSH_SRC_SRC},
    {"and",     AND_SRC_SRC},
    {"or",      OR_SRC_SRC},
    {"xor",     XOR_SRC_SRC},
};

#endif // DEREKS_ASSEMBLY_H
