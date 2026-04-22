// Kinda based on my vibe of ARM ASM, but really reduced, and soimplified. Not optimized, the real world doesn't work like this.
// This will be an enum for the isntructions, which can get packet with other things to fit in the program counter, and also have associated string so I can compile to text assembly, then I'll pack that
#ifndef ISADEFS_H
#define ISADEFS_H
#include <assert.h>


/* Format of the instruction:

Every instruction is a 64 bit number, split into these parts

(I want to make this as simple as possible), and I can afford to
*/

typedef uint8_t Opcode;
enum {
    INVALID = 0,
    NOOP,
    LOAD,
    STORE,
    B,
    BEQ,
    BNEQ,
    CMP,

    MOV_REG,
    MOV_IMM_TO_LO,
    MOV_IMM_TO_HI,

    ADD_SRC_SRC,
    ADD_SRC_IMM,
    SUB_SRC_SRC,
    SUB_SRC_IMM,
    MUL_SRC_SRC,
    MUL_SRC_IMM,
    DIV_SRC_SRC,
    DIV_SRC_IMM,
    MOD_SRC_SRC,
    MOD_SRC_IMM,
    LSH_SRC_SRC,
    LSH_SRC_IMM,
    RSH_SRC_SRC,
    RSH_SRC_IMM,
    AND_SRC_SRC,
    AND_SRC_IMM,
    OR_SRC_SRC,
    OR_SRC_IMM,
    XOR_SRC_SRC,
    XOR_SRC_IMM,

    ISA_COUNT,
};


typedef struct Instruction {
    Opcode opcode;
    uint8_t regA;// Destination
    uint8_t regB;
    uint8_t regC;
    uint32_t val;
} Instruction;

typedef union InstructionBits {
    uint64_t raw;
    Instruction ins;
} InstructionBits;

static_assert(ISA_COUNT < 256, "Too many instructions");
static_assert(sizeof(Instruction) == 8, "Instruction struct must be 8 bytes");
static_assert(sizeof(InstructionBits) == 8, "InstructionBits union must be 8 bytes");


typedef uint64_t MyStatus;
enum {
    EQUAL = 0ULL,
    LESS = 1ULL,
    GREATER = 2ULL
};


typedef uint64_t ProgramState;
enum {
    NOT_STARTED = 0ULL,
    RUNNING = 1ULL,
    FAILED = 2ULL
};
/*  Core Instructions I'll need (IDK all the instructions that exist, but this should ge tme starts)
 * ext// exit program // An ISA doesn't need to include this, but it came to mind to make it easier to emulate,
 *      and as thius is for fun/klearning, not production, I'll use it to see what happens, maybe this will be fun

    Other notes
 * I'll need to have either numbers, or addresses , or annother register, as a value in a register,
 *   how to know? I need a flag or 2here
*/

// for now, I have 64 bits for instruction, I wouldn't ever need that many instuctions, so I can pack some other data
// this is where I want to put the info about the type of value in the register

// byte0 byte1 byte2 byte3
// byte0: Instruction first byte
// byre1: Instruction sedcond byte
// byte2: first nibble (two bits) represent what type of value (raw number (0), address in ram (1), register number (2)) first register holds,
// byre3: I don't need this yet, maybe extend it?


#endif // ISADEFS_H