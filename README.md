# Sometimes I get the itch to write some of these really cool things
I wasn't intending to spend time on this, just have a quarentine to get it thorugh my fingers, but it's captured my interest more the more I've worked.
I'm jumping in the deep end before reading the textbook/ manual, but will go for inspiration here or there in the moment.

## All pieces of the project
- my own (ARM inspired) limited ISA
- an assembler to turn this assembly into a binary file
- a compiler (real C to assembly for my ISA)
- An emulator to run this binary on
- A debugger
- Automated testsing for a file or for a set of files
    - The same C source file is compiled by gcc and ran on my PC, compiled.
    - Either case can take input fron stdin and output to stdout
    - I think I'll also end up allowing it to take argc argv

# Reference
- https://developer.arm.com/documentation/dui0231/b/arm-instruction-reference


# How to test it out 
## Assembler
gcc src/Assembler.cpp src/Assembler_helpers.c -o assembler

## Processor/ emulator/ debugger program
gcc src/processor.c src/implementInstructions.c src/Assembler_helpers.c src/os_like_stuff.c -o proc

## run:
./assembler testFiles/some_basic_instructions.dasm -o output
./proc output --debug # then run `P` and press <enter> to see the registers

## use dasm in the CPU directly
./proc output --dasm
```dasm
>>mov gp1, 9
```