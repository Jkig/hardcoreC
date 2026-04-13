#ifndef ASSEMBLER_HELPERS_H
#define ASSEMBLER_HELPERS_H

#include <stdint.h>
#include <stdlib.h>

#include "ISADef.h"
#include "Assembler.h"

#ifdef __cplusplus
extern "C" {
#endif

// Only things needed for interactive assembly execution

int lookup_value(const StringToInt *, size_t, const char *, uint8_t *);
void parse_line(const char *line, InstructionStrings *);
int is_immediate(const char *);
int get_reg_number(const char *);
void print_instruction(Instruction);
Instruction build_one_binary_instruction(const char *);

#ifdef __cplusplus
}
#endif

#endif // ASSEMBLER_HELPERS_H
