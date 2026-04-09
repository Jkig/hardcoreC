# This file shows what a binary that'll run on my processor will look like

0x00000000: Initial stack pointer

0x00000008: Start of the vector table (reset vector is here)
...
0x00000200: End of the vector table (last entry)

0x00000208: Start of memory after the vector table

Next secitons are all variable in size. 

I'll have the stack grow down (from the top of the )

# I do have to decide how much memory I can alllow for this, should be fine, I'll just take up like half what gcc gives me for emulate processor.c, leaving the rest for the processor's operations

This is really making me understand the feelling of the old timers, that instructions and data is all the same thing, and the runtime altering of code seems more resonable from here (I know its not, ofc, thats insane (these days, with cheap hardware))


- A note on the vector table, I definitely don't need this many, honestly I only care about the rest vecotor for now, I'd be very satisfied if I could run a program that takes a # n as input from the consol, and calculates the nth fibinacci number and prints it out. That's really cool, I don't need to get an OS on here, or emulate other hardware I/O. Serial to a terminal should be good, but might as well give myself the felxibility


## What I need to store somewhere:
- text (code)
- intitialized statics and globals
- uninitialized statics and globals
- the stack
- the heap

After all the space the statics and globals need, I start putting funciton definitions

After all the space for the code, we have the stack growing down

Theoredically, I'd be using the heap, it shouldn't be abolutely crazy to implement a simple version, but I'll probably 
* Probs initiallize the heap with a max size, and just keep a free list, starting as []
* This malloc and free funcitonality will be implemented directly into the executable, having its own data structure (I suppose manged on the heap, maybe set aside space for it for the initial version)


# Notes:
* last part of compilation is setting the reset vector to the start of main, and the initial stack pointer to right before main
* TODO: how to pass args to main? For an easy solution, I could set aside a fixed hunk of space before the stack for any passed parameters.
* I'm not trying to be to "embedded brained" so remember that. Interrupts don't matter too much.

# Reference manual (lol)
- LSB
- big endian?/ little endian? think about what I want to target, I is implementation defined, and lots change, 
  - looks like little endian is most common on arm
