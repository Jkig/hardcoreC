#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "ISADef.h"

#define MAX_TOKEN 32
#define MAX_ASM_LINE_LENGTH 64


typedef struct {
    const char *key;
    uint8_t value;
} StringToInt;


typedef struct {
    char op[MAX_TOKEN];
    char arg1[MAX_TOKEN];
    char arg2[MAX_TOKEN];
    char arg3[MAX_TOKEN];
    char arg4[MAX_TOKEN];
} InstructionStrings;

void assemble(const char *, const char *);

#endif // ASSEMBLER_H