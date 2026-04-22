## Program layout
- text
    - The vector table
    - Then the program
- Intitialized statics and globals
- ~~uninitialized statics and globals~~ (Not yet)
- The stack
- ~~The heap~~ (not yet)

### Vector table
0x00000000: Initial stack pointer
0x00000008: Start of the vector table (reset vector is here)
...
0x00000200: End of the vector table (last entry)
0x00000208: Start of memory after the vector table


# I think I'll set aside a max of 100kb of space on the stack for this program to run out of
- I'll have the stack grow down (from the top of the )


# Heap implementation
It shouldn't be too hard to implement a simple version, but I'll hold off for now
- This malloc and free funcitonality will be implemented directly into the executable, having its own data structure (I suppose manged on the heap, maybe set aside space for it for the initial version)

# .bss
I'm doing this project for fun and to learn. If I can make the data section, I can do the .bss. 

# Notes:
- last part of compilation is setting the reset vector to the start of main, and the initial stack pointer to right before main
- TODO: how to pass args to main? For an easy solution, I could set aside a fixed hunk of space before the stack for any passed parameters.
- I'm not trying to be to "embedded brained" so remember that. Interrupts don't matter too much.

# Reference manual (lol)
- LSB
- big endian?/ little endian? think about what I want to target, I is implementation defined, and lots change, 
  - looks like little endian is most common on arm
