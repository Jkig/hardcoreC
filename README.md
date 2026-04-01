# Sometimes I get the itch to write some of these really cool things
This isn't a project I'll devote time to, but this is a place I can at least get it thorugh my fingers and move back to productive work. Sometimes these topics capture my interest.

## Technically I should do the lowest level work first, but again, I'm doing what I have/want to first
write some specs out, then they can expend in the future, separate the notes from the code a bit more.

## I'm thinking I want to Design
- my own (ARM inspired) limited ISA
- an assembler to turn this assembly into a binary file
- a compiler (real C to assembly for my ISA)
- An emulator to run this binary on 
- A debugger

I want to be able to run automated tests on all the files in the test programs, same C source file is compiled by gcc and ran on my PC, then Derek compiled (by my compiler, which is a c file compiled by gcc, then ran on my processor). Run a leetcode problem, maybe solve sudoku, diff mine vs standard?

## To keep it simple
* Maybe all variables are at the start of the function?
* Basically no optimisation.
- Only one C file in, only one dasm out of compiler, only one binary file from that, ran by itself.
- I'll implement a limited version of c

### Whats not included in my C
- goto
- heap
- any stdlib except for stdint for now
- Main can't take parameters ofc
