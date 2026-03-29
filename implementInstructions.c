// I'll write C code that implements the instructions

// Just a list of funcitons and what they do to registers and memory
// Every function takes the same thing? The remaining part of the instructions?
#include "emulateProcessor.h"

instruciton_table[ISA_COUNT] = {ADD, SUB, JMP};//{ADD, SUB, MUL, DIV, LSH, RSH, LOAD, STORE, JMP, CMP};// list of these funciton pointers

// They all just operate on the global register set, but some do have info with code, ex add r0 r1 #5


void ADD() {
    *res = src1 + src2;
}

void SUB() {
    *res = src1 - src2;
}

void JMP() {
    *pc = addr;
}