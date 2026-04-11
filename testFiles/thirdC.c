#include <stdio.h>
#include <stdint.h>

// as my compiler doesn't do anything with macros -> this is the way to print variables
#define print_int(a) printf("%ld", (a))
// Try a global


int32_t add_something(int32_t a, int32_t b) {
    printf("adding some things together\n");
    printf("a: ");
    print_int(a);
    printf(" b: ");
    print_int(b);
    printf("\n");
    return a + b;
}

int main(void) {
    uint32_t firstvar = 1;
    uint32_t endval = 0;

    endval = add_something(firstvar, 8);
    print_int(endval);
}
