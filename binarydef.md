# This file shows what a binary that'll run on my processor will look like

0x00000000: Initial stack pointer

0x00000001: Start of the vector table (reset vector is here)
0x0000001F: End of the vector table (last entry)

0x00000020: Start of memoery after the vector table (I think I'll put intitialized statics and globals, then uninitialized statics and globals (I know in the real world, these are seperate because we don't want to allocate space in the binary, but its not important for what I'm doing, so uninitialized will effectively be the same as initialzied, but initialized to 0))


After all the space the statics and globals need, I start putting funciton definitions

After all the space for the code, we have the stack growing down

Theoredically, I'd be using the heap, it shouldn't be abolutely crazy to implement a simple version, but I'll probably 
* Probs initiallize the heap with a max size, and just keep a free list, starting as []
* This malloc and free funcitonality will be implemented directly into the executable, having its own data structure (I suppose manged on the heap, maybe set aside space for it for the initial version)


# Notes:
* last part of compilation is setting the reset vector to the start of main, and the initial stack pointer to right before main
* TODO: how to pass args to main? For an easy solution, I could set aside a fixed hunk of space before the stack for any passed parameters.
* I'm not trying to be to "embedded brained" so remember that. Interrupts don't matter too much.

