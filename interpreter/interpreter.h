#pragma once

#include <stdlib.h>


typedef long long word;

typedef struct
{
    size_t address;
    word   accumulator;
    size_t memory_size;
    word*  memory;
} interpreter;


typedef enum
{
    ERR_INTERPRETER_INIT_SUCCESS,
    ERR_INTERPRETER_INIT_MALLOC
} err_interpreter_init;

typedef enum
{
    ERR_INTERPRETER_NEXT_SUCCESS,
    ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_INSTRUCTION,
    ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_ACCESS
} err_interpreter_next;


err_interpreter_init interpreter_init(interpreter* p, size_t memory_size);
err_interpreter_next interpreter_next(interpreter* p);
void                 interpreter_free(interpreter* p);
