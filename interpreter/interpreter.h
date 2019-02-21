#pragma once

#include <stdlib.h>

typedef struct
{
    size_t         address;
    long long      accumulator;
    size_t         memory_size;
    unsigned char* memory;
} interpreter_t;

int  interpreter_init(interpreter_t* p, size_t memory_size);
int  interpreter_next(interpreter_t* p);
void interpreter_free(interpreter_t* p);
