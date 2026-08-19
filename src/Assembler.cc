#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Assembler.h"
#include "Assembler_helpers.h"
#include "ISADef.h"
#include "processor.h"


void assemble(const char *input_file, const char *output_file) {
  FILE *dasm = fopen(input_file, "r");
  FILE *binary = fopen(output_file, "wb");

  // To start, super limited version
  // I'm using the stack yet, but I'll put it far out
  uint64_t placeholder = sizeof(uint64_t) * 100;
  fwrite(&placeholder, sizeof(uint64_t), 1, binary);

  // hardcode _start right after vector table, and have reset vector point
  // there
  placeholder = sizeof(uint64_t) * (INTERRUPT_COUNT + 1);
  fwrite(&placeholder, sizeof(uint64_t), 1, binary);

  placeholder = 0; // clear out the rest of the vector table
  for (uint8_t i = 0; i < INTERRUPT_COUNT - 1 - 2; i++)
    fwrite(&placeholder, sizeof(uint64_t), 1, binary);

  const char *text = "programmed!!Hi!!";
  fwrite(text, 1, strlen(text), binary);

  // Start writing after vector table, I'm hardcoding start to go here.
  char line[MAX_ASM_LINE_LENGTH];
  InstructionBits full = {0};
  while (fgets(line, sizeof(line), dasm)) {
    line[strcspn(line, "\n")] = '\0';
    if (strnlen(line, MAX_ASM_LINE_LENGTH) < 2)// not just \n
      continue;
    
    full.ins = build_one_binary_instruction(line);
    fwrite(&full.ins, sizeof(uint64_t), 1, binary);
  }

  fclose(dasm);
  fclose(binary);
}


int main(int argc, char *argv[]) {
  // Goes from .dasm (Derek's assembly) file to a binary file
  // TODO: think about how I do the increment instruction for when I move in
  // larget than 32 bit immediates, basically its just 2 instructions, not
  // too complicated, just have to increment the running offset
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
        printf("Usage: %s <file_name>.c -o <binary_file_name>\n",
            argv[0]);
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
