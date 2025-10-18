# Sometimes I get the itch to write a compilier, or other pieces of the puzzle. I'm not making this a project I'll devote time to, but this is a place I can at least get it thorugh my fingers and move back to productive work

I'm thinking I want to Design:
* my own (ARM inspired) limited ISA
* an assembler
* a compiler (real C to my ISA, ran on my software defined processor)

I want to be able to run automated tests on all the files in the test programs, same C source file is compiled by gcc and ran on my PC, then Derek compiled (by my compiler, which is a c file compiled by gcc, then ran on my processor). Run a leetcode problem, maybe solve sudoku, diff mine vs standard?

To keep it simple:
* Maybe all definitions are at the start of the function?
* Basically no optimisation? Its not production on purpose
* Pass in file name of my compiled binary to a file into the emulator….
