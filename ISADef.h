// Kinda based on my vibe of ARM ASM, but really reduced, and soimplified. Not optimized, the real world doesn't work like this.
// This will be an enum for the isntructions, which can get packet with other things to fit in the program counter, and also have associated string so I can compile to text assembly, then I'll pack that
#ifndef ISADEFS_H
#define ISADEFS_H


typedef uint64_t MyISA;
enum {
    ADD = 1ULL,
    SUB = 2ULL,
    MUL = 3ULL,
    DIV = 4ULL,
    LSH = 5ULL,
    RSH = 6ULL,
    LOAD = 7ULL,
    STORE = 8ULL,
    JMP = 9ULL,
    CMP = 10ULL,
    ISA_COUNT,
};


typedef uint64_t MyStatus;
enum {
   EQUAL = 0ULL,
   LESS = 1ULL,
   GREATER = 2ULL
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