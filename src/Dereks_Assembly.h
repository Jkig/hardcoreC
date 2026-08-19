// If you are exploring, this is the place to start, this is what you can use
// to write your .dasm programs
#ifndef DEREKS_ASSEMBLY_H
#define DEREKS_ASSEMBLY_H
#include "Assembler.h"
#include "processor.h"

/*******************************************************************************
 See .dasm files in testFiles for example usage
 See binaryDef_and_usage.md for the "reference maual"
 ******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Register names
////////////////////////////////////////////////////////////////////////////////

StringToInt REGISTER_NAMES[] = {
    {"pc",      REG_PC},
    {"sp",      REG_SP},
    {"status",  REG_STATUS},
    {"gp",      GP_REGISTERS_OFFSET},
};


////////////////////////////////////////////////////////////////////////////////
// Instructions
////////////////////////////////////////////////////////////////////////////////

StringToInt direct_opcodes[] = {
    {"noop",    NOOP},
    {"load",    LOAD},
    {"store",   STORE},
    {"b",       B},
    {"beq",     BEQ},
    {"bneq",    BNEQ},// TODO: I think I want to get rid of this, I'm keeping
                      // it streamlined
};

/* All instructions after this are really multiple opcodes, I'll call them
 * variations
 *    For example, you can move an immediate value into some destination
 *    register, or you can move some the contents of some other register into
 *    the destination register
 *
 * Variations opcode numbers will always start with src, then immediate
 */
StringToInt mov[] = {
    {"mov",     MOV_REG},
    // Mov has 3 variations, src -> dst, immediate to lowhalf of dst or
    // highhalf of dst
};

StringToInt cmp[] = {
    {"cmp",     CMP_SRC},
};

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
