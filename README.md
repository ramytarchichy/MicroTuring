# MicroTuring

This is a single-instruction turing-complete interpreter and compiler, inspired by [this Wikipedia article](https://en.wikipedia.org/wiki/One_instruction_set_computer). It was initially made to explore turing-completeness and instructions, allow the user to flex his creative and problem-solving muscles, and ultimately alleviate boredom. It was later intentionally made to be a lot more restrictive in terms of speed and memory, and 10 times more fun!

## Interpreter

### Instruction

MicroTuring runs a modified version of [Subleq+](https://esolangs.org/wiki/Subleq%2B), which allows for self-modification and turing-completeness. The instruction is called `micro`. In pseudocode, it looks like this:

    micro (a, b):
        acc -= mem[a]
        mem[a] = acc
        if (acc >= 0):
            jmp b

The instruction takes advantage of an internal register called `acc` (accumulator) to reduce the number of parameters to 2: by consequence, more instructions and cycles are needed to perform the same computations, and the code will become more confusing (see [Turing tarpit](https://en.wikipedia.org/wiki/Turing_tarpit)), so that should make it even more fun. It also has an address register (obviously), but I'm going to ignore it.

By default, the speed is limited to roughly 1 MHz (1 million instructions per second).

### Memory

The memory space is divided into `word`s (`long long`s). It is unified, with absolutely no memory safety for any program running on it (the interpreter itself and the system it's running on are still safe). `micro` takes 2 arguments, `a` and `b`, both of which are `word`s, making each instruction take up 2 `word`s, unless you're doing something really funky.

Note that since the size of a `long long` may vary depending on the system architecture, MicroTuring programs will need to be recompiled for different architectures too.

By default, the interpreter has a memory size of 64K `word`s.

### I/O

//TODO

## Compiler

    clr:
        micro zero >
        micro zero >

    setzero: a
        micro a >
        micro a >

    jmp: addr
        micro zero >
        micro zero addr

    add: a b result ;adds two numbers
        clr
        
        setzero result
        micro a >

    .zero 0

Note that while macros may look like functions, they are not. Whenever "called", they are simply replaced by the contents of the macro definition. Therefore, they do not allow for any recursion, and don't save any space (nor do they take up space either). They are simply there for the programmer's convenience.

It is recommended to define all variables at the end of the file, as the value's position in memory when added to the generated file depends on its position in the file, and may result in the variable being unintentionally executed as code.
