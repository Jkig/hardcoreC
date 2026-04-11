Add "syscalls" with a software interrupt
- one instruction, result in some register (or status?)
    - take a register with a pointer (read/write, or nothing)
    - syscall mumber. (Include read/writes)
    right now, all I need is read and write from user, will pass this to the actual processor so I can take input
    -> just whole lines into / out of fixed size buffers, keep it simple for now.

Order of operations to implement the rest of the operations in
0. load
1. store
2. compare
3. jump, conditional, etc..??

- When to start on cpp, just a little bit of stuff, linked list with vectors of descendant functions is super helpful.

Order of operations to implement the c language in
0. basic math stuff?
1. if
2. while
...
function calls
- I can be very inneficient, even thinking of it like a psudo context switch, always easier to optimize something that exists.
	- variables are byte offsets/ sizes from the top of the stack???? IDK
    - think about to go to and return from a function, I don't think it's too bad
	- chop the end of the stack off
	- I think there is one instruction for going to a funciton, the rest of the set up in inserted first, function calls don't exist in dasm
	- jump back to the earlier instruction, only one return value, so  returns are just updating some registers…
	- Set aside some registers to only do this??? still call them gp?
