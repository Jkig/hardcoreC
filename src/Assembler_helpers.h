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

int lookup_value(const StringToInt *table, size_t count, const char *key,
    uint8_t *out_value);
void parse_line(const char *line, InstructionStrings *inst);
int is_immediate(const char *s);
int get_reg_number(const char *s);
void print_instruction(Instruction sample);
Instruction build_one_binary_instruction(const char *line);

#ifdef __cplusplus
}
#endif

#endif // ASSEMBLER_HELPERS_H
