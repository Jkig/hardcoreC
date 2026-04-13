// this isn't too bad
//64 bit architecture
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdalign.h>
#include <string.h>

#include "Assembler.h"
#include "Assembler_helpers.h"
#include "binaryDef.h"
#include "implementInstructions.h"
#include "ISADef.h"
#include "os_like_stuff.h"
#include "processor.h"

// I'll only have 2 real syscalls, in/out, this program can catch them, in and out through the console that I'm actually using
// Could trigger interrupts manually from annother thread later


// External globals
extern instruction instruction_table[ISA_COUNT];


// Globals
Registers registers;
alignas(8) uint8_t ram[RAM_SIZE];
volatile uint64_t interrupt_signals;    // bit field
uint64_t *vtable_start = (uint64_t *) (uint64_t) &ram[8];
ProgramState program_state = NOT_STARTED;// TODO: Is this needed?
uint64_t program_return = 0;



void print_regs() {
    printf("PC: 0x%016llx, SP: 0x%016llx, status: 0x%016llx, Res: 0x%016llx\n", registers.pc, registers.sp, registers.status, registers.res);
    for (uint8_t i=0;i<GENERAL_PURPOSE_REGISTER_COUNT/2;i++) {// I don't need to see everything
        printf("\tgp[%d]:\t0x%016llx\t%lld\n", i, registers.gp[i], registers.gp[i]);
    }
    printf("\n");
}


void executeInterrupts() {
    // lowest will be highest priority if there are multiple, so lowest address in the vector table first
    // Its a bare metal program, so the user gets to define the interrupts, but I have to do the work to context switch here (like hardware would)
    
    // Set up for the context switch, I don't have to do it in .dasm, "the hardware does it"
    // Just dump the registers on the stack and restore, its actually easier than normal

    for (uint8_t voffset=0; voffset<INTERUPT_COUNT;voffset++) {
        while ((interrupt_signals & (1 << voffset)) != 0) {
            interrupt_signals &= ~(1 << voffset);// Turn off the interrupt that triggered this, move this into the interrupt implementetion if its ever needed to not happen
            if (voffset <= SYSTICK)
                registers.pc = vtable_start[voffset]; // bare metal interrupts, allow programmer to specify
            else
                interrupt(voffset);// OS would catch these, but right now I'm implemeintg them in a way that feels like hw?
            // Technically this means I've left most of the vector table unreachable, that's fine, I have way more interrupts than I need
        }
        // then execute until it returns? this still needs work
    }
    // Switch context back
}

void execute() {
    InstructionBits raw_instruction = {0};
    if (registers.pc >= (uint64_t) &ram[RAM_SIZE]) {
        printf("Congrats on the segfault!\n");
        printf("Tried to access an instruction outside of ram\n");
    }
    memcpy(&raw_instruction.raw, &ram[registers.pc], sizeof(Instruction));
    print_instruction(raw_instruction.ins);
    instruction_table[raw_instruction.ins.opcode]();// I/O is a bit special, but I just put it in here
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


int debug(bool allow_dasm) {
    // TODO: latere I'll implement a lot larger debug mode, but for now just step, print registers, and 
    // adding a full on GDB isn't absolutely crazy hard. For now just print a list of addresses for the functions, expand it later
    
    // I think I want a couple levels of debuging, one for later is more for debugging C, and I want to be able to place anything in any register.
    
    printf(".");
    int cmd;
    do {
        cmd = getchar();
    } while (cmd == '\n' || cmd == '\r');

    switch(cmd) {
        case 'P':
            print_regs();
            break;
        case 'p':
            // read address from user and print the contents of that address in RAM
            printf("Enter address to print: ");
            uint64_t addr;
            scanf("%llx", &addr);
            printf("Contents at address %016llx: %016llx\n", addr, *(uint64_t *)(&ram[addr]));
            break;
        case 's':
            // Step through the next instruction
            break;
        case 'r':
            // print register states
            break;
        case 'k':
            // terminate program
            return -1;
        case '>':
            if (!allow_dasm) {
                printf("not allowed to run dasm right now\n");
                break;
            }

            char c;
            int i = 0;
            char dasm_cmd[MAX_ASM_LINE_LENGTH] = {0};
            while ((c = getchar()) != '\n' && c != EOF) {
                if (i < MAX_ASM_LINE_LENGTH - 1) {
                    dasm_cmd[i++] = (char)c;
                }
            }

            InstructionBits next_instruction = {0};
            next_instruction.ins = build_one_binary_instruction(&dasm_cmd[0]);
            if (next_instruction.raw == 0) {
                printf("Invalid command\n");
                break;
            }

            registers.pc = 0;
            memcpy(&ram[registers.pc], &next_instruction.raw, sizeof(Instruction));
            break;
        default:
            break;
    }
    printf("\n");
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\nProvide the name of a binary file\n", argv[0]);
        return 1;
    }

    bool debug_mode = false;
    bool dasm_interpereter = false;
    if (argc > 2) {
        if (strcmp(argv[2], "--debug") == 0) {
            debug_mode = true;
        } else if (strcmp(argv[2], "--dasm") == 0) {
            debug_mode = true;
            dasm_interpereter = true;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[2]);
            fprintf(stderr, "Usage: %s <filename> [--debug]\nProvide the name of a binary file and optionally enable debug mode", argv[0]);
            return 1;
        }
    }

    size_t loaded = load_file_to_ram(argv[1]);
    if (loaded == 0 && !dasm_interpereter) {
        fprintf(stderr, "Failed to load file into RAM\nEmpty binary file or read error\n");
        return 1;
    }

    memcpy(&registers.sp, &ram[0], sizeof(uint64_t));
    memcpy(&registers.pc, &ram[sizeof(uint64_t)], sizeof(uint64_t));

    while (true) {
        if (debug_mode && debug(dasm_interpereter)) break;

        while (interrupt_signals != 0) executeInterrupts();

        execute();
        // if (program_state != 1) break;
    }

    /* Give me a chance to explore after program stops
    while (true) {
        if (debug(false)) break;
    }
    */
    return program_return;
}
