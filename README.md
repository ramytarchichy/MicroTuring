# MicroTuring

This is a single-instruction turing-complete interpreter, inspired by [this Wikipedia article](https://en.wikipedia.org/wiki/One_instruction_set_computer). It was made to explore turing-completeness and instruction sets, and also alleviate boredom.

## Interpreter

### Instruction

MicroTuring runs a modified version of [Subleq+](https://esolangs.org/wiki/Subleq%2B), which allows for self-modification and turing-completeness. We call this instruction `micro`. In pseudocode, it looks like this:

    micro (a, b):
        acc -= memory[a]
        memory[a] = acc
        if (acc >= 0):
            jmp b

The instruction takes advantage of an internal register called `acc` (accumulator) to reduce the number of parameters to 2: by consequence, more instructions and cycles are needed to perform the same computations, and the code will become more confusing (see [Turing tarpit](https://en.wikipedia.org/wiki/Turing_tarpit)). It also has an address register (obviously), but we're going to ignore it.
