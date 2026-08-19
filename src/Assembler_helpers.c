#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Assembler.h"
#include "Dereks_Assembly.h"
#include "ISADef.h"
#include "processor.h"



#ifdef __cplusplus
extern "C" {
#endif

int lookup_value(const StringToInt *table, size_t count, const char *key,
                 uint8_t *out_value) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(table[i].key, key) == 0) {
            *out_value = table[i].value;
            return 1;
        }
    }
    return 0;
}



void parse_line(const char *line, InstructionStrings *inst) {
    memset(inst, 0, sizeof(InstructionStrings));

    if (line[0] != '\t' && line[0] != '>') {
        printf("shouldn't have seen a line that wasn't starting with \\t "
               "in this function\n");
        return;
    }

    const char *p = line;

    while (*p == ' ' || *p == '\t' || *p == '>') p++;

    char *fields[5] = {
        inst->op,
        inst->arg1,
        inst->arg2,
        inst->arg3,
        inst->arg4
    };

    int field = 0;
    int idx = 0;

    while (*p && field < 5) {
        if (*p == ';') break;

        if (*p == ' ' || *p == '\t' || *p == ',') {
            if (idx > 0) {
                // terminate current token
                fields[field][idx] = '\0';

                field++;
                idx = 0;
            }
            p++;
            continue;
        }

        if (idx < MAX_TOKEN - 1) {
            fields[field][idx++] = *p;
        }

        p++;
    }

    // finalize last token if line didn't end with delimiter
    if (field < 5 && idx > 0) {
        fields[field][idx] = '\0';
    }
}

int is_immediate(const char *s) {
    if (!s || s[0] == '\0') return 0;

    // handle negative numbers too
    if (s[0] == '-') {
        return isdigit((unsigned char)s[1]);
    }

    return isdigit((unsigned char)s[0]);
}

int get_reg_number(const char *s) {
    uint8_t register_number = 0;
    if (s[0] == 'g' && s[1] == 'p')
        return atoi(&s[2]) + GP_REGISTERS_OFFSET;

    if (lookup_value(REGISTER_NAMES,
                     sizeof(REGISTER_NAMES) / sizeof(StringToInt),
                     s, &register_number))
        return register_number;
    return 0;
}

// maybe I make an intermediate that goes to the real op, and reorders, and
// space sepearteds the arguments
// zero is an invalid instruction
Instruction build_one_binary_instruction(const char *line) {
    Instruction invalid_res = {0};
    
    InstructionStrings elements;
    parse_line(line, &elements);
    if (elements.op[0] == '\0') {
        printf("No operation string?\n");
        return invalid_res;
    }

    uint8_t opcode;
    Instruction res = {0};
    if (lookup_value(arithmetic_instruction_to_opcode,
                     sizeof(arithmetic_instruction_to_opcode) /
                         sizeof(StringToInt),
                     elements.op, &opcode)) {
        if (elements.arg1[0] == '\0' || elements.arg2[0] == '\0' ||
            elements.arg2[0] == '\0') {
            printf("Not all needed args for arithmetic operations");
            return invalid_res;
        }
        res.regA = get_reg_number(elements.arg1);
        res.regB = get_reg_number(elements.arg2);

        if (is_immediate(elements.arg3)) {
            // This is a nice trick, becuase my arithmetic ops are always
            // defined in number as _src then _imm
            opcode++;
            res.val = atoi(elements.arg3);// TODO: bounds check, own func...
        } else {
            res.regC = get_reg_number(elements.arg3);
        }
        res.opcode = opcode;

        return res;

    } else if (lookup_value(direct_opcodes,
                            sizeof(direct_opcodes) / sizeof(StringToInt),
                            elements.op, &opcode)) {
        res.opcode = opcode;
        // these are largely all special?
        return res;
        
    } else if (lookup_value(mov, sizeof(mov) / sizeof(StringToInt),
                            elements.op, &opcode)) {
        if (elements.arg1[0] == '\0' || elements.arg2[0] == '\0') {
            printf("Not all needed args for mov");
            return invalid_res;
        }
        
        res.regA = get_reg_number(elements.arg1);

        if (is_immediate(elements.arg2)) {
            // This is a nice trick, becuase my arithmetic ops are always
            // defined in number as _src then _imm
            opcode++;
            // TODO: either add mov HI, or split it in a few chunks. Right now
            // just MOV_LO
            // TODO: This is one of the small changes I make between human
            // readable asm and about to go to binary
            res.val = atoi(elements.arg2);// TODO: bounds check, own func...
        } else {
            res.regB = get_reg_number(elements.arg2);
        }
        res.opcode = opcode;
        
        return res;
    } else if (lookup_value(cmp, sizeof(cmp) / sizeof(StringToInt),
                            elements.op, &opcode)) {
        if (elements.arg1[0] == '\0' || elements.arg2[0] == '\0') {
            printf("Not all needed args for cmp");
            return invalid_res;
        }
        
        res.regA = get_reg_number(elements.arg1);

        if (is_immediate(elements.arg2)) {
            // This is a nice trick, becuase my arithmetic ops are always
            // defined in number as _src then _imm
            opcode++;
            res.val = atoi(elements.arg2);// TODO: bounds check, own func...
        } else {
            res.regB = get_reg_number(elements.arg2);
        }
        res.opcode = opcode;
        
        return res;
    }
    printf("Nothing done, opcode not found, line: %s\n", line);
    return invalid_res;
}

void print_instruction(Instruction sample) {
    printf("opcode: 0x%02x, A: 0x%02x, B: 0x%02x, C: 0x%02x, "
           "Val: 0x%08x\n",
           sample.opcode, sample.regA, sample.regB, sample.regC, sample.val);
}

#ifdef __cplusplus
}
#endif
