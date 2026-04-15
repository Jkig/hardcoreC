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

- I don't need an DAG for the functions, recursion is fine
	- Just be able to branch to a function, or referene something in ram / in data seciton. -> if its in a function, there is a set value 
	- I could have everything be done in registers, this is simple, but too restrictive??? maybe say fuck it??
	- at what phase to branch to that?
- Data is similar, swap every reference of it to the data
- -> how to use types??
- I think maybe I need readchar?? build the rest out of it?
- Every line starting with `'\t'` is a single instruction. set up to replace each thing with the address I'll write it to, which is just the start.

- make an automated way to test these programs,,, on main, make a syscall for argv/argc, then judge prints. all info is taken on argc argv
