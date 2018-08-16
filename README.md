# MicroTuring

No off-the-shelf lexer/parser generators used, everything was built from scratch. Do whatever you want with it.

This is a single-instruction turing-complete interpreter and compiler in C++, inspired by a [Wikipedia article](https://en.wikipedia.org/wiki/One_instruction_set_computer). It was made to explore turing-completeness, instruction sets, and to just mess around.

## Interpreter

### Instruction Set

MicroTuring runs a sightly tweaked version of [SUBLEQ+](https://esolangs.org/wiki/Subleq%2B), which allows for self-modification and turing-completeness. We call this instruction `uSUBGEQ+`. In pseudocode, it looks like this:

    usubgeq+ (a, b):
        ACC -= Mem[a]
        Mem[a] = ACC
        if (ACC >= 0):
            JMP b

The instruction takes advantage of an internal register called `ACC` (accumulator) to reduce the number of parameters from 3 to 2 (by consequence, the number of total instructions increases, and you have to be a bit more creative, but who doesn't like a fun challenge?). It also technically has an address register obviously, but that is not directly accessible by the user and so we're not going to talk about it.

`a` and `b` are memory locations, starting from 0, in this case implemented as 64-bit signed integers for simplicity, since data stored in memory can also be signed.

These memory addresses point to a single 64-bit integer, meaning that a single instruction takes up 2 memory locations. The interpreter will start with an `ACC` value of 0, and an address of 0, taking every 2 integers and processing them as the parameters of the instruction, then moving to the next 2, unless the instruction causes a jump, doing so until the address is out of bounds.

### Usage

Linux: `./i <filename>`

## Compiler

__**NOTE:**__ COMPILER IS WIP, YOU'LL HAVE TO BUILD YOUR PROGRAMS BY HAND FOR NOW.

Since using a hex editor to manually type those instruction parameters in is a pain in the lower back area, an (ugly) compiler was included in this repository to simplify programming the interpreter. The only 2 things it really does is eliminate copy-pasting the same code all over the place, and having to keep track of addresses.

### Language Syntax

Each of the following elements have to be placed on a separate line, except comments. Empty lines, extra spaces and indentation are allowed.

* `i <a> <b>` is the `uSUBGEQ+` instruction. It doesn't take raw integers, only data and addresses marked with the syntax below.
* `>` is a keyword that refers to the next instruction's address. Using it as a macro "argument" will return the address of the instruction after the last item in the macro.
* `END` is a keyword that refers to the last address in memory. Jumping to it will cause the program to stop.
* `*<name> <value>` will create a constant that can be used as a raw integer, similar to `#define` in C.
* `$<name> <value>` adds a data variable that can be referred to. Value must be a raw integer and cannot refer to other data. Be careful where you place this, as it can be executed as code.
* `%<name> [size=0] [value0=0] [value1=value0] [value2=value1] [value3=value2] ...` creates multiple consecutive variables (array) that default to the last explicit value, or 0 if none exist, and can be referred to with `array~index`. The index must be a raw integer, example: `stack~2`. This can be used to access raw memory beyond the maximum size of the array.
* `:<name>` marks the element after it as a memory location you can point to, meant strictly for jumps.
* `#<text>` creates a single-line comment.
* `#[<text>]#` creates a multiline comment block.
* `!<name> [params]` creates a macro that will be inlined into the bytecode. This is NOT a function and cannot do recursion, as there is no stack (you have to do that on your own). It does allow overloading, so `!sub a b` is different from `!sub a b c`, but is the same as `!sub b c`. Nested macro definitions are not allowed. Macro blocks are specified by indentation, Python-style.
* `<name> [args]` inlines a macro.
* `@<file>` inlines a file, similar to `#include` in C.

### Usage

Linux: `./icc <input_file> [output_file = out]`
