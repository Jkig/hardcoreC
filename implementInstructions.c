// I'll write C code that implements the instructions

// Just a list of funcitons and what they do to registers and memory
// Every function takes the same thing? The remaining part of the instructions?
#include "emulateProcessor.h"
#include "implementInstructions.h"
#include "ISADef.h"
#include <stdint.h>


// External globals
extern uint64_t program_state;
extern uint64_t program_return = 0;
extern _registers registers;
extern uint8_t ram[RAM_SIZE];


// Globals
instruction instruction_table[ISA_COUNT] = {ADD, SUB, JMP};//{ADD, SUB, MUL, DIV, LSH, RSH, LOAD, STORE, JMP, CMP};// list of these funciton pointers



void ADD() {
    *res = src1 + src2;
}

void SUB() {
    *res = src1 - src2;
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

