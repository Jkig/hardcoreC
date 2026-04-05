#ifndef EMULATE_PROCESSOR_H
#define EMULATE_PROCESSOR_H


#include <stdint.h>

#define RAM_SIZE (1024 * 1024)    // 1 mb of ram
#define GENERAL_PURPOSE_REGISTER_COUNT  16

typedef struct {
    uint64_t pc;
    uint64_t sp;
    uint64_t res;
    uint64_t status;
    uint64_t gp[GENERAL_PURPOSE_REGISTER_COUNT];
} Registers;

typedef enum {
    REG_PC = 1,
    REG_SP,
    REG_RES,
    REG_STATUS,
    GP_REGISTER_OFFSETS// General purpose arrays will be in an array here
} RegisterId;

#define LAST_REGISTER_BEFORE_GP (GP_REGISTER_OFFSETS - 1)

#endif // EMULATE_PROCESSOR_H
