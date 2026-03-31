// this isn't too bad
//64 bit architecture
#include "emulateProcessor.h"
#include "implementInstructions.h"
#include "ISADef.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// I won't emulate a real processor, I'll only get close enought to really solidify the idea


// I'll only have 2 real syscalls, in/out, this program can catch them, in and out through the console that I'm actually using
// Could trigger interrupts manually from annother thread later



// External globals
extern instruction instruciton_table[ISA_COUNT];




// GLOBALS:
_registers registers;
volatile uint64_t interrupt_signals;    // I have 64 possible interrupts
uint8_t ram[RAM_SIZE];
uint64_t *vtable_start = &ram[8];
uint64_t program_state = 0;// 0 is not started (or finished successfully), 1 is running, anything else is some sort of error or other return value
uint64_t program_return = 0;


void executeInterrupts() {
    // execute any pendinge interrupts, and execute it, leftmost will be highest priority
    // I think really the only interrupt will be "Trap to the OS to printf()"
    // I'll sprintf into a buffer, then use a single instruction pointing 
    // these are not the same vectors? IDK what I'll do yet
    // actually the registers on the stack, I don't really care about interrupts yet

    // Its a bare metal program, so it gets to define the interrupts, but I have to do the work to context switch here (like hardware would)
    
    // Set up for the context switch, I don't have to do it in .dsm, "the hardware does it"
    // I should also implement a software interrupt instruction to go here! its not too hard

    for (uint8_t voffset=0; voffset<64;voffset++) {
        while (interrupt_signals & (1 << voffset) != 0) // its the interrupts job to disable itself?
            registers.pc = vtable_start[voffset];  
    }

    // Switch context back
}

void execute() {
    // Impliment all the instructuions here, map them to fucncitons in c that do things
    // maps this instruction to a c function i'll define


    // wait until I have access to all the values in the registers I'll need for this
    // I can, so I'll spare a few registsers (i'm making them up lol) to hold intermediates
    instruciton_table[registers.pc]();// I'm pssing here and there, globals everyone has, because who cares, I don't think I'll ever build this
}

// Function to load file into RAM
size_t load_file_to_ram(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return 0;
    }

    // Read up to RAM_SIZE bytes
    size_t bytes_read = fread(ram, 1, RAM_SIZE, file);

    if (ferror(file)) {
        perror("fread");
        fclose(file);
        return 0;
    }

    fclose(file);
    return bytes_read;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\nProvide the name of a binary file", argv[0]);
        return 1;
    }
    bool debug_mode = false;
    if (argc > 2) {
        if (strcmp(argv[2], "--debug") == 0) {
            debug_mode = true;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[2]);
            fprintf(stderr, "Usage: %s <filename> [--debug]\nProvide the name of a binary file and optionally enable debug mode", argv[0]);
            return 1;
        }
    }

    size_t loaded = load_file_to_ram(argv[1]);
    if (loaded == 0) {
        fprintf(stderr, "Failed to load file into RAM\nEmpty binary file or read error\n");
        return 1;
    }

    registers.sp = *(uint64_t *)(&ram[0]);
    registers.pc = *(uint64_t *)(&ram[8]);

    while (1) {
        if (debug_mode) {
            // TODO: latere I'll implement a lot larger debug mode, but for now just step, print registers, and 
            printf(".");
            char cmd = fgetchar();
            switch(cmd) {
                case 'P':
                    // Print the contents of all registers
                    printf("PC: %016lx, SP: %016lx, r1-r4: %016lx, %016lx, %016lx, %016lx\n", registers.pc, registers.sp, registers.r1, registers.r2, registers.r3, registers.r4);
                    break;
                case 'p':
                    // read address from user and print the contents of that address in RAM
                    uint64_t addr;
                    printf("Enter address to print: ");
                    scanf("%lx", &addr);
                    printf("Contents at address %016lx: %016lx\n", addr, *(uint64_t *)(&ram[addr]));
                    break;
                case 's':
                    // Step through the next instruction
                    break;
                case 'r':
                    // print register states
                    break;
                default:
                    break;
                printf("\n");
        }

        // poll the interrupts
        // do them one after the other
        while (interrupt_signals != 0) {
            executeInterrupt();// call the right interrupt handler
        }

        // do the next instrucition
        execute();// I/O is a bit special, but I just put it in here
        if (program_state != 1)
            break;

    }
    return program_return;
}




// Now I should bring in a binary from something else
/* What would it mean to actually build this project

 * I wouldn't be actually implimenting the digital logic in hardware, or even FPGA, or even emulated in C, I'll just make the 
 * ISA, and run it in the C program, I would also need to write a linker from C, and a c -> my ISA bassed assembly/assember
 *      (this step could easilly be one, I should be able to go from )
 * to limit the scope, I could just allow a single file

 * and, as a fun, funny, joke, I can write the compiler/ assember in python. parse it, etc (I forgot about this one XD)
*/


// What does the resume entry look like:

/* the original project was: I wrote my own ISA, a compiler for it in (python or C), and implimented the core in FPGA, this is cracted to the absolute max

    The current in contrast:
 * Wrote my own ISA, as well as a compiler for it in python, an emulator for it in C, and tested that the code resulted in the same
 * Program's outputs should have be the same as the exact same C source code compiled with gcc...
 * in either case, just print the results, impliment a leetcode medium, and some good test cases..

 * scope out this project, write it up, and don't do it, but if I ever give up on being rich, and hate my life, I could do this
 * IDK how bad it is, what instuctions do I need? probs not that bad...

*/


/* Perhaps just impliment a limited version of c, no goto, no heap
 * 

*/