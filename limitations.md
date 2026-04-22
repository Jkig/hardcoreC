## To keep it simple
* All variables are at the start of the function?
* No optimisations.
- Only one C file in, only one dasm out of compiler, only one binary file from that, ran by itself.

### Whats not included in my C (yet?)
- heap
- Main can't take parameters
- All statements need {} (no `if (something) printf("wow\n");`)
- No formatted strings in printf
- any stdlib except for stdint/stdbool, and a bit of sdtio
- many of the keywords - (if I can make this work, I could spend a ton of time and implement the rest, I probably won't care to. What am I going to learn implementing switch that I didn't learn implementing while)
- Be careful how you name things, I will swap stuff out blindly
- No floats!

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
