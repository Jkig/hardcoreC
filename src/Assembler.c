#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Assembler.h"
#include "binaryDef.h"
#include "ISADef.h"
#include "processor.h"


int main(int argc, char *argv[]) {
    // Goes from .dasm (Derek's assembly) file to a binary file
    // take a file name etc..
    const char *input_file = NULL;
    const char *output_file = NULL;

    int i = 1; // skip argv[0]

    while (i < argc) {
        char *arg = argv[i];

        if (strcmp(arg, "-o") == 0) {
            i++;
            if (i < argc) {
                output_file = argv[i];
            } else {
                printf("Usage: %s <file_name>.c -o <binary_file_name>\n", argv[0]);
                return 1;
            }
        } else {
            // assume it's the input file
            if (input_file == NULL) {
                input_file = arg;
            } else {
                printf("Unknown argument: %s\n", arg);
                return 1;
            }
        }
        i++;
    }

    if (input_file == NULL || output_file == NULL) {
        printf("File names not set\n");
        printf("   input_file: %s\n   output_file: %s\n",
               input_file ? input_file : "NULL",
               output_file ? output_file : "NULL");
        return 1;
    }

    assemble(input_file, output_file);

    return 0;
}
