
buf = bytearray(16)

buf[4:8] = b'\xde\xad\xbe\xef'

REGISTER_NAMES = {
    "pc",
    "sp",
    "res",
    "status",

    "gp0",
    "gp1",
    "gp2",
    "gp3",
    "gp4",
    "gp5",
    "gp6",
    "gp7",
    "gp8",
    "gp9",
    "gp10",
    "gp11",
    "gp12",
    "gp13",
    "gp14",
    "gp15",
}

INSTRUCTIONS = {
    "noop",
    "mov",
    "add",
    "sub",
    "mul",
    "div",
    "mod",
    "lsh",
    "rsh",
    "and",
    "or",
    "xor",
    "xnor",

    "load",
    "store",
    "jmp",
    "cmp",
}


# maybe I make an intermediate that goes to the real op, and reorders, and space sepearteds the arguments
def oneline_binary(line: str) -> bytearray:
    '''
    
    '''
    output_instruction = bytearray(16)
    REGISTER_NAMES
    
    return output_instruction


if __name__ == "__main__":

    oneline_binary("\tmov gp1, 1234")
    oneline_binary("\tmov gp1, gp2")