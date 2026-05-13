import os
import sys


class ASTNode:
    def __init__(self, node_type, func_name=None):
        self.node_type = node_type
        self.func_name = func_name
        self.children = []
        self.code = ""
        # TODO: Is the easiest way to build the code here after I've built the AST?


unused_keywords = {
    "break",
    "case",
    "continue",
    "const",
    "do",
    "double",
    "enum",
    "extern",
    "float",
    "for",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "unsigned",
    "union",
    "void",
    "volatile",
}


def tokenize_file(input_filename, output_filename):
    # this function is largely GPT written, anyone can do this

    try:
        input_file = open(input_filename, "r")
    except IOError:
        print("Error opening input file")
        return

    try:
        output_file = open(output_filename, "w")
    except IOError:
        print("Error opening output file")
        input_file.close()
        return

    def peek():
        pos = input_file.tell()
        c = input_file.read(1)
        input_file.seek(pos)
        return c

    def peek2():
        pos = input_file.tell()
        c = input_file.read(2)
        input_file.seek(pos)
        return c

    def write_token(tok):
        output_file.write(tok + " ")

    while True:
        c = input_file.read(1)
        if c == "":
            break  # EOF

        # --- skip whitespace ---
        if c.isspace():
            continue

        # --- skip comments ---
        if c == '/':
            next_char = peek()

            if next_char == '/':  # single-line
                input_file.read(1)  # consume second '/'
                while True:
                    c = input_file.read(1)
                    if c == "" or c == '\n':
                        break
                continue

            elif next_char == '*':  # multi-line
                input_file.read(1)  # consume '*'
                while True:
                    c = input_file.read(1)
                    if c == "":
                        break
                    if c == '*' and peek() == '/':
                        input_file.read(1)  # consume '/'
                        break
                continue

            else:
                write_token('/')
                continue

        # --- identifier / keyword ---
        if c.isalpha() or c == '_':
            token = c
            while True:
                nxt = peek()
                if nxt.isalnum() or nxt == '_':
                    token += input_file.read(1)
                else:
                    break
            write_token(token)
            continue

        # --- number (simple integers for now) ---
        if c.isdigit():
            token = c
            while True:
                nxt = peek()
                if nxt.isdigit():
                    token += input_file.read(1)
                else:
                    break
            write_token(token)
            continue

        # --- multi-char operators ---
        two = c + peek()
        if two in {"==", "!=", "<=", ">=", "&&", "||", "++", "--", "->"}:
            input_file.read(1)  # consume second char
            write_token(two)
            continue

        # --- single-char operators / punctuation ---
        if c in "{}()[]:;,.+-*/%=<>!?&|^~":
            write_token(c)
            continue

        # --- skip preprocessor directives ---
        if c == '#':
            while True:
                c = input_file.read(1)
                if c == "" or c == '\n':
                    break
            continue

        # --- char literal ---
        if c == "'":
            token = c

            while True:
                nxt = input_file.read(1)
                if nxt == "":
                    raise ValueError("Unterminated char literal")

                token += nxt

                if nxt == '\\':
                    escaped = input_file.read(1)
                    if escaped == "":
                        raise ValueError("Unterminated char literal")
                    token += escaped
                    continue

                if nxt == "'":
                    break

            write_token(token)
            continue

        # --- string literal ---
        if c == '"':
            token = c

            while True:
                nxt = input_file.read(1)
                if nxt == "":
                    raise ValueError("Unterminated string literal")

                token += nxt

                if nxt == '\\':
                    escaped = input_file.read(1)
                    if escaped == "":
                        raise ValueError("Unterminated string literal")
                    token += escaped
                    continue

                if nxt == '"':
                    break

            write_token(token)
            continue

        raise ValueError(f"Unexpected character: {c}")

    input_file.close()
    output_file.close()


def to_dasm(root_node: ASTNode, file_name: str):
    '''
    Takes a root node of the AST with .dasm lines, and generates dasm code for it, writing to a .dasm file
    (easiest to do this in multiple passes?) is it fine to go through it line by line? what does an AST do?
    # If I don't have sufficient instructions in my ISA I'll find out here
    # I need some more information, like the globals and statics before I can build the file.
    # order to work on these things in:
        - Pulling all statics, globals, and literals to the right place - 2-4 hours?
        - (types) (I have no idea) (4 hours?)
        - arithmetic operations, including =, etc. (4 hours)
        - if, while (4 hours)
        - boolean stuff (4 hours)
        - Any function call? - 4 hours?
    '''

    dasm_file_name = ".".join(file_name.split(".")[:-1] + ["dasm"])
    tokenized_file_name = ".".join(file_name.split(".")[:-1] + ["tokenized"])
    
    try:
        input_file = open(tokenized_file_name, "r")
    except IOError:
        print("Error opening input file")
        return

    try:
        output_file = open(dasm_file_name, "w")
    except IOError:
        print("Error opening output file")
        input_file.close()
        return
    
    
    input_file.close()
    output_file.close()


def build_AST(tokenized_file_name) -> ASTNode:
    '''
    ## Throws an error if not possible to build an AST 
    - (e.g. syntax error
    - funciton not found
    - cyclic dependencies
    
    ## Notes
    # I can always start from main, this will be the root of the AST
    '''
    pass


def compile(file_name: str, output_file_name: str, skip_cleanup: bool):
    tokenized_file_name = ".".join(file_name.split(".")[:-1] + ["tokenized"])
    tokenize_file(file_name, tokenized_file_name) # is this the right term?

    content = ""
    with open(tokenized_file_name, "r") as f:
        content = f.read()

    all_content = set(content.split())
    non_implemented_keywords = False
    for keyword in unused_keywords:
        if keyword in all_content:
            non_implemented_keywords = True
            print(f"Found {keyword} in program, not implemented yet!")

    if non_implemented_keywords:
        return

    # strip and replace a bunch of stuff
    content = content.replace("true ", "1 ")
    content = content.replace("false ", "0 ")

    content = content.replace("unsigned long long ", "uint64_t ")
    content = content.replace("long long ", "int64_t ")
    content = content.replace("unsigned long ", "int32_t ")
    content = content.replace("long ", "int32_t ")
    content = content.replace("unsigned short ", "uint16_t ")
    content = content.replace("short ", "int16_t ")
    content = content.replace("unsigned char ", "uint8_t ")
    content = content.replace("char ", "int8_t ")
    content = content.replace("bool ", "uint8_t ")

    content = content.replace("int main ", "start_function_int_main ")
    content = content.replace("unsigned int ", "uint32_t ")
    content = content.replace("int ", "int32_t ")
    content = content.replace("start_function_int_main ", "int main ")

    with open(tokenized_file_name, "w") as f:
        f.write(content)

    root_node = None
    try:
        root_node = build_AST(tokenized_file_name)
    except Exception as e:
        print(f"Error building AST: {e}")
        print(f"Cleanup skipped")
        return
    


    to_dasm(root_node, file_name)

    # undeceided on if this should be done in c or maybe just here in python
    # assemble_binary(file_name, output_file_name) # Implmement it in both sides, can I get the enum and such from ctypes?

    if not skip_cleanup:
        os.remove(tokenized_file_name)


if __name__ == "__main__":
    input_file = None
    output_file = None
    save_temps = False
    args = sys.argv[1:]

    i = 0
    while i < len(args):
        arg = args[i]
        print(arg)
        if arg == "-o":
            i += 1
            if i < len(args):
                output_file = args[i]
            else:
                print("Usage: python3 Compiler.py <file_name>.c -o <binary_file_name> [options]")
                print("   -save-temps: don't delete the temporary files")
                sys.exit(1)
        elif arg == "-save-temps":
            save_temps = True
        else:
            # assume it's the input file
            if input_file is None:
                input_file = arg
            else:
                print(f"Unknown argument: {arg}")
                sys.exit(1)
        i += 1
    
    if input_file is None or output_file is None:
        print(sys.argv)
        print(f"File names not set")
        print(f"   input_file: {input_file}\n   output_file: {output_file}")
        sys.exit(1)

    compile(input_file, output_file, save_temps)

