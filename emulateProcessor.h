#ifndef EMULATE_PROCESSOR_H
#define EMULATE_PROCESSOR_H


#include <stdint.h>

#define RAM_SIZE (1024 * 1024)    // 1 mb of ram

struct _registers {
    uint64_t pc;
    uint64_t sp;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t r4;
    uint64_t res;
    uint64_t status
};
typedef struct _registers _registers;

#endif // EMULATE_PROCESSOR_H
