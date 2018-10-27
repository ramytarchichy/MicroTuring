# MicroTuring

This is a single-instruction turing-complete interpreter and compiler in C++, inspired by [this Wikipedia article](https://en.wikipedia.org/wiki/One_instruction_set_computer). It was made to explore turing-completeness, instruction sets, and alleviate boredom.

## Interpreter

### Instruction Set

MicroTuring runs a modified version of [Subleq+](https://esolangs.org/wiki/Subleq%2B), which allows for self-modification and turing-completeness. We call this instruction `micro`. In pseudocode, it looks like this:

    micro (a, b):
        acc -= memory[a]
        memory[a] = acc
        if (acc >= 0):
            jmp b

The instruction takes advantage of an internal register called `acc` (accumulator) to reduce the number of parameters to 2: by consequence, more instructions and cycles are needed to perform the same computations, and the code will become more confusing (see [Turing tarpit](https://en.wikipedia.org/wiki/Turing_tarpit)). It also has an address register (obviously), but we're going to ignore it.

The memory space is an array of GMP arbitrary-precision rational numbers (`mpq_class`) because it allows for rational arithmetic and storage of rational numbers. However, the `a` and `b` arguments passed to the instruction, being memory addresses, must be positive integers (rationals with a denominator of 1) smaller than 2^64, including 0.

Each memory address points to a single rational number, meaning that a single instruction takes up 2 memory locations. The interpreter will start with an `acc` value of 0, and an address of 0, taking every 2 rational numbers and passing them to the instruction, then moving to the next 2 (unless the instruction causes a jump).

### Usage

Linux: `./mrun <filename>`

## Compiler

__**NOTE:**__ COMPILER IS WIP, YOU'LL HAVE TO BUILD YOUR PROGRAMS BY HAND FOR NOW.
