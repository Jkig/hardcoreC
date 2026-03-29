#include <stdio.h>
#include "../emulateProcessor.h"


int main(void) {
    uint8_t data[RAM_SIZE] = {0};

    FILE *file = fopen("minimal_instructions_test.fakeBin", "wb");// Doesn't follow my real program format, but the processor will execute it regardless
    if (!file) {
        perror("fopen");
        return 1;
    }

    data[0] = *(uint64_t *)(&ram[0]);
    registers.pc = *(uint64_t *)(&ram[8]);


    size_t written = fwrite(data, sizeof(uint8_t), RAM_SIZE, file);
    if (written != RAM_SIZE) {
        perror("fwrite");
    }

    fclose(file);

    return 0;
}