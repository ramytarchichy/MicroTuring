#pragma once

#include <stdlib.h>


typedef struct
{
    size_t     address;
    long long  accumulator;
    size_t     memory_size;
    long long* memory;
} interpreter_t;


typedef enum
{
    ERR_INTERPRETER_INIT_SUCCESS,
    ERR_INTERPRETER_INIT_MALLOC
} err_interpreter_init_t;

typedef enum
{
    ERR_INTERPRETER_NEXT_SUCCESS,
    ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_INSTRUCTION,
    ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_ACCESS
} err_interpreter_next_t;


err_interpreter_init_t interpreter_init(interpreter_t* p, size_t memory_size);
err_interpreter_next_t interpreter_next(interpreter_t* p);
void                   interpreter_free(interpreter_t* p);
