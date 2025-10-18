// this isn't too bad
// 32 bit architecture

// I won't emulate a real processor, I'll only get close enought to really solidify the idea

int program_state = 0;// 0 is not started (or finished successfully), 1 is running, anything else is some sort of error or other return value
int program_return = 0;


typedef struct _registers {int pc, int sp, int r1, int r2, int r3, int res, int status};
typedef struct _registers registers;
unsigned int interrupt_signals;  // I have 32 possible interrupts

// interrupts:
// interrupt 0
void reset_vec() {
    program_state = 1;
}

// interrupt 1
void NMI_vec() {
    
}

// interrupt 2
void hardfault_vec() {
    
}
// the rest are as follows, not to crazy



int add(int src, int dst, int val) {
    return;
}





void *vtable[reset_vec, NMI_vec, hardfault_vec];// list of these funciton pointers
void *instruciton_table[add];// list of these funciton pointers




void executeInterrupts() {
    // execute any pendinge interrupts, and execute it, leftmost will be highest priority
    // I think really the only interrupt will be "Trap to the OS to printf()"
    // I'll sprintf into a buffer, then use a single instruction pointing 
    // these are not the same vectors? IDK what I'll do yet
    for (unsigned char voffset = 0, voffset<32;voffset++) {
        while (interrupt_signals & (1 << voffset) != 0) // its the interrupts job to disable itself
            vtable[voffset]();
    }
}

void execute(_registers curr_registers) {
    // Impliment all the instructuions here, map them to fucncitons in c that do things
    // maps this instruction to a c function i'll define


    // wait until I have access to all the values in the registers I'll need for this
    // I can, so I'll spare a few registsers (i'm making them up lol) to hold intermediates
    instruciton_table[curr_registers.pc](curr_registers);// I'm pssing here and there, globals everyone has, because who cares, I don't think I'll ever build this
}

int main(void) {
    typedef struct _registers saved_registers;
    
    reset_vec();

    while (1) {
        // poll the interrupts
        // do them one after the other
        if (interrupt_signals != 0) {
            saved_registers = registers;// make sure copying right

            while (interrupt_signals != 0) {
                executeInterrupt();// call the right interrupt handler
            }
            registers = saved_registers;
        }

        // do the next instrucition
        execute(registers.pc);
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
 * results as the exact same C source code, compiled with gcc...
 * in either case, just print the results, impliment a leetcode medium, and some good test cases..

 * my one question is how much is this to impliment, I can use the normal c

 * scope out this project, write it up, and don't do it, but if I ever give up on being rich, and hate my life, I could do this
 * IDK how bad it is, what instuctions do I need? probs not that bad...

*/


/* Perhaps just impliment a limited version of c, no goto, no heap
 * 

*/