#include "implementInstructions.h"
#include "os_like_stuff.h"
#include "processor.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// External globals
extern uint64_t program_state;
extern uint64_t program_return;
extern Registers registers;
extern volatile uint64_t interrupt_signals;    // bit field
extern uint8_t ram[RAM_SIZE_BYTES];

// Not locking myself itno this, but when a softare interrupt happens
void interrupt(uint8_t interrupt_no) {
    if (interrupt_no != SOFTWARE_INTERRUPT_LOW_1) {
        printf("I've only implemented SOFTWARE_INTERRUPT_LOW_1 for software interrupts thus far");
        return;
    }

    uint64_t interrupt_sub_number       = registers.gp[INTERRUPT_SUB_NUMBER];
    uint64_t interrupt_auxiliary_info   = registers.gp[INTERRUPT_AUX_NUMBER];
    uint64_t read_buff                  = registers.gp[INTERRUPT_POINTER_1];
    uint64_t write_buff                 = registers.gp[INTERRUPT_POINTER_2];

    if (interrupt_sub_number == READ) {
        if (interrupt_auxiliary_info == SDT_IN) {
            char c;
            int i = 0;
            while ((c = getchar()) != '\n' && c != EOF) {
                if (i < 640 - 1) {
                    ram[read_buff + i++] = (char)c;// TODO this is untested
                }
            }
            ram[i++] = '\0';
        }
    } else if(interrupt_sub_number == WRITE) {
        if (interrupt_auxiliary_info == STD_OUT) {
            printf("> %s", &ram[write_buff]);// hopefully it null termitates lol
        }
    } else {
        printf("Not implemented");
        return;
    }
}
