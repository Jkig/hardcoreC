#include <stdio.h>
#include <stdint.h>

// This is the starting point, but I'll build on this as time goes on, trying a
// global, trying an array, trying an else if



int main(void) {
  uint8_t firstvar = 1;

  if (firstvar > 0) {
    printf("wow, something kinda worked!!\n");
  } else {
    printf("Something broke?\n");
    return 1;
  }
  return 0;
}
