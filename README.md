# Sometimes I get the itch to write some of these really cool things
This isn't a project I'll devote time to, but this is a place I can at least get it thorugh my fingers and move back to productive work. Sometimes these topics capture my interest.
I'm jumping in the deep end before reading the textbook/ manual, but will go for inspiration here or there in the moment.

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
- heap
- Main can't take parameters
- any stdlib except for stdint/stdbool, and a bit of sdtio
- many of the keywords - (if I can make this work, I could spend a ton of time and implement the rest, I probably won't care to. What am I going to learn implementing switch that I didn't learn implementing while)

### Limitations in my ASM
- All imediate values are decimal

#### The above aren't crazy undertakings, but I'll start simple

# To Study
- asm for a function call
- opcode definition?
- is an ast for all potential jumps? just functions, or something else?
- start with sample ASM files, and test manually -> the simple assembler

# Potential fun notes, not needed
### potential mistakes
I'll have different variations as different core instruciotns, not a few bits for it.



# Build
## Assembler
gcc Assembler.c -o assembler

## Processor/ emulator/ debugger program
gcc processor.c implementInstructions.c -o proc

## run:
./assembler testFiles/some_basic_instructions.dasm -o output
./proc output --debug # then I like P <enter> to see the registers