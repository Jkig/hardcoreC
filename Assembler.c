#include <stdint.h>
#include "ISADef.h"
#include "processor.h"

typedef struct {
    const char *key;
    uint8_t value;
} StringToInt;

StringToInt REGISTER_NAMES[] = {
    {"pc", REG_PC},
    {"sp", REG_SP},
    {"res", REG_RES},
    {"status", REG_STATUS},
    {"gp", GP_REGISTER_OFFSETS},
};


Instructions_To[] = {
    "noop",
    "mov",

    "load",
    "store",
    "jmp",
    "cmp",
};



// All either src or IMM, based on if the third param is a number
// src is first, then if it is imm add 1
StringToInt arithmetic_instruction_to_opcode[] = {
    {"add", ADD_SRC_SRC},
    {"sub", SUB_SRC_SRC},
    {"mul", MUL_SRC_SRC},
    {"div", DIV_SRC_SRC},
    {"mod", MOD_SRC_SRC},
    {"lsh", LSH_SRC_SRC},
    {"rsh", RSH_SRC_SRC},
    {"and", AND_SRC_SRC},
    {"or", OR_SRC_SRC},
    {"xor", XOR_SRC_SRC},
};


// maybe I make an intermediate that goes to the real op, and reorders, and space sepearteds the arguments
Instruction oneline_binary(const char *line) {
    Instruction res;
    // if its in arithmetic, find it and variation
    // etc etc..

    return res;
}


int main() {
    moneline_binary("\tmov gp1, 1234");
    oneline_binary("\tmov gp1, gp2");
    // Goes from .sasm (Derek's assembly) file to a binary file
    return 0;
    // take a file name etc..

}
