
## The problem

Computers don't execute high level language code like python

```Python
a = math.sqrt(49)
```

Or even lower level languages like C

```C
int32_t a = 15;
int32_t b = 17 * a;
```

Or even actual low level languages, like assembly

```arm_asm
tst r0, #1  
bne odd_path ; if LSB = 1 → odd  
  
even_path:  
mov r1, #1  
b done  
  
odd_path:  
mov r2, #1  
  
done:
```

But raw binary

```hex
4869 2066 7269 656e 642c 2074 656c 6c20 6d65 2069 6620 796f 7520 666f 756e 6420 7468 6973 203a 29
```

That was hex :)

raw binary

```binary_bytes
01001000 01101001 00100000 01100110 01110010 01101001 01100101 01101110 01100100 00101100 00100000 01110100 01100101 01101100 01101100 00100000 01101101 01100101 00100000 01101001 01100110 00100000 01111001 01101111 01110101 00100000 01100110 01101111 01110101 01101110 01100100 00100000 01110100 01101000 01101001 01110011 00100000 00111010 00101001
```

There aren't any spaces

```binary
010010000110100100100000011001100111001001101001011001010110111001100100001011000010000001110100011001010110110001101100001000000110110101100101001000000110100101100110001000000111100101101111011101010010000001100110011011110111010101101110011001000010000001110100011010000110100101110011001000000011101000101001
```


## The solution

by convention bytes are 8 bits, and we don't address smaller units, and we may have a register size of 64 (this is what I'm using) -> think of this as the natural size of a number for this computer. And hex is just a way to display it to us, so by these conventions we can think of it like:

```hex
4869 2066 7269 656e
642c 2074 656c 6c20
6d65 2069 6620 796f
7520 666f 756e 6420
```

each of these lines being one thing the computer does


## talk about how my actual thing works

use excalidraw, show a real instruction -> just a number, does something


then how an actual CPU really works (show mine)


walk through a single instruction (ADD)


now it seems more natural how we can take this add instruction and build this, there is register and source..

so the asembler take this text and goes down until a binary the computer cna actually run

but this progam still isn't super readable 
(btw, this isn't all the way there.  still running simplified Derek's assembly (.dasm))
```dasm
	mov gp4, 9
	mov gp2, 97
	add gp1, gp2, gp4
	mov gp2, gp1
	mul gp1, gp2, 13
```


so we build this out of a compiler, being able to write actually readily readable c code

this is the cool think I wanted to do
