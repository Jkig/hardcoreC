#ifndef EMULATE_PROCESSOR_H
#define EMULATE_PROCESSOR_H
#include <assert.h>


#include <stdint.h>

#define RAM_SIZE                        (1024 * 1024) // 1 mb of ram
#define GENERAL_PURPOSE_REGISTER_COUNT  16

#define INTERRUPT_SUB_NUMBER            8
#define INTERRUPT_AUX_NUMBER            9
#define INTERRUPT_POINTER_1             10
#define INTERRUPT_POINTER_2             11

typedef enum {
    CONTINUE_PROGRAM,
    SKIP_INSTRUCTION,
    END_PROGRAM
} program_actions;


typedef struct {
    uint64_t pc;
    uint64_t sp;
    uint64_t res;
    uint64_t status;
    uint64_t gp[GENERAL_PURPOSE_REGISTER_COUNT];
} Registers;

typedef enum {
    INVALID_REGISTER,
    REG_PC,
    REG_SP,
    REG_RES,
    REG_STATUS,
    GP_REGISTERS_OFFSET// General purpose arrays will be in an array here, going from [gp0, gp<GENERAL_PURPOSE_REGISTER_COUNT-1>] inclusive  
} RegisterId;

typedef enum {
    ZERO        = (1 << 0),
    NEGATIVE    = (1 << 1),
    CARRY       = (1 << 2),
    OVERFLOW    = (1 << 3)
} StatusRegisterBits;
// some interrupts will be used for certain puprposes commonly

// TODO: Same for function stuff
// put as much on the stack as I can?? maybe everything except the first number, which is the count of params?


typedef enum {
    RESET,
    NON_MASKABLE,
    HARDFAULT,
    MEM_FAULT,
    BUS_FAULT,
    SOFT_FAULT_0,
    SOFT_FAULT_1,
    SOFT_FAULT_2,
    SOFT_FAULT_3,
    SOFTWARE_INTERRUPT_HI_0,
    SOFTWARE_INTERRUPT_HI_1,
    SOFTWARE_INTERRUPT_HI_2,
    SOFTWARE_INTERRUPT_HI_3,
    SYSTICK,
    SYSTICK_EXT,
    SOFTWARE_INTERRUPT_LOW_0,
    SOFTWARE_INTERRUPT_LOW_1,
    SOFTWARE_INTERRUPT_LOW_2,
    SOFTWARE_INTERRUPT_LOW_3,

// Done with core interrupts, now handing some more to peropherals, OS, etc...?
    PERIPHERAL_I2C,
    PERIPHERAL_SPI,
    PERIPHERAL_GPIO,
    PERIPHERAL_PCIE,
    PERIPHERAL_DEBUGGER,
    INTERRUPT_COUNT,
} Interrupts;

typedef enum {
    UNUSED,
    READ,
    WRITE,
} Software_Interrupts_low_1;


typedef enum {
    SDT_IN,
    STD_OUT,
    STD_ERR,
} File_descriptors;


#define LAST_REGISTER_BEFORE_GP (GP_REGISTERS_OFFSET - 1)
static_assert(INTERRUPT_COUNT <= 64, "Too many interrupts");

#endif // EMULATE_PROCESSOR_H
