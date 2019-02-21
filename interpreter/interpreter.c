#include "interpreter.h"


err_interpreter_init_t interpreter_init(interpreter_t* p, size_t memory_size)
{
    p->address     = 0;
    p->accumulator = 0;
    p->memory_size = memory_size;
    p->memory      = malloc(memory_size * sizeof *p->memory);

    if (p->memory == NULL) return ERR_INTERPRETER_INIT_MALLOC;
    else                   return ERR_INTERPRETER_INIT_SUCCESS;
}


err_interpreter_next_t interpreter_next(interpreter_t* p)
{
    if (p->address + 2 * sizeof(long long) > p->memory_size)
        return ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_INSTRUCTION;

    const long long a = *(long long*)&p->memory[p->address];
    const long long b = *(long long*)&p->memory[p->address + sizeof a];

    if (a + sizeof(long long) > p->memory_size)
        return ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_ACCESS;

    p->accumulator -= *(long long*)&p->memory[a];
    *(long long*)&p->memory[a] = p->accumulator;

    if (p->accumulator >= 0)
    {
        //Yes it's technically undefined behavior if `b` is negative
        //No I don't care
        p->address = b;
    }

    return ERR_INTERPRETER_NEXT_SUCCESS;
}


void interpreter_free(interpreter_t* p)
{
    free(p->memory);
}
