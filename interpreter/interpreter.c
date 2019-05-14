#include "interpreter.h"


err_interpreter_init interpreter_init(interpreter* p, size_t memory_size)
{
    p->address     = 0;
    p->accumulator = 0;
    p->memory_size = memory_size;
    p->memory      = malloc(memory_size * sizeof *p->memory);

    if (p->memory == NULL) return ERR_INTERPRETER_INIT_MALLOC;
    else                   return ERR_INTERPRETER_INIT_SUCCESS;
}


err_interpreter_next interpreter_next(interpreter* p)
{
    if (p->address + 2 > p->memory_size)
        return ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_INSTRUCTION;

    const word a = p->memory[p->address];
    const word b = p->memory[p->address+1];

    if (a > p->memory_size)
        return ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_ACCESS;

    p->accumulator -= p->memory[a];
    p->memory[a] = p->accumulator;

    if (p->accumulator >= 0)
    {
        //Yes it's technically undefined behavior if `b` is negative
        //No I don't care, it's more fun for the user to debug
        p->address = b;
    }

    return ERR_INTERPRETER_NEXT_SUCCESS;
}


void interpreter_free(interpreter* p)
{
    free(p->memory);
}
