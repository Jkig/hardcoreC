## To keep it simple
* All variables are at the start of the function?
* No optimisations.
- Only one C file in, only one dasm out of compiler, only one binary file from that, ran by itself.
- File 
- I'll start by not actually having anything on the stack, all "locals" will actually go to some global, then get local variables working, globals should be easier, start there.
- Do local variables remain offsets from the pointer to the current function frame, should I keep a register for top of the current stack? then ability to restore?

### Whats not included in my C (yet?)
- heap
- Main can't take parameters
- All statements need {} (no `if (something) printf("wow\n");`)
- No formatted strings in printf
- any stdlib except for stdint/stdbool, and a bit of sdtio
- many of the keywords - (I won't spend the time to implement the rest of C. I'm doing this to learn, what am I going to learn implementing `switch` that I didn't learn implementing `if`, or implementing `for` that I didn't learn implementing `while`)
- **See the `unused_keywords` in Compiler.py** 
- Be careful how you name things, I will swap stuff out blindly
- No floats!
- No statics, file or function level
- All variables must be initialized
- no forward declarations
- wait a bit to cast

### Limitations in my ASM
- All imediate values are decimal
- All variables are char arrays or 64 bit
later I'll add more:
- char (1 byte), hword (2 byte), word (4 bytes), llong (8 bytes)
- no .bss, its important in real programs (not so much these days), but not valuable for my leraning IMO
- one file only
- Be careful how you name things, I will swap stuff out blindly
- its done in 2 passes, everything can see everyone else's addresses at any time

#### The above aren't crazy undertakings, but I'll start simple
