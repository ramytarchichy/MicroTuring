#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "interpreter.h"


int read_file_to_memory(const char* file_path, unsigned char** data, size_t* size)
{
    //Open wordlist in read-only mode
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) return -1;

    //Get size
    int error = fseek(file, 0L, SEEK_END);
    if (error != 0) return -1;

    size_t s = ftell(file);
    if (s == -1L) return -1;

    rewind(file);

    //Save file contents to memory
    *size = s;
    *data = malloc(s * sizeof **data);

    error = fread(*data, 1, s, file);

    //Wordlist is saved in memory, we can now close the file
    error = fclose(file);
    if (error != 0) return -1;

    return 0;
}


int main(int argc, const char** argv)
{
    if (argc != 1)
    {
        //TODO: print error
        return EXIT_FAILURE;
    }

    //Settings
    char* file_path = argv[1];

    //Machine
    interpreter_t cpu;
    int resultInitCpu = interpreter_init(&cpu, 16 * 1024 * 1024);
    if (!resultInitCpu)
    {
        //TODO: print error
        return EXIT_FAILURE;
    }


    //Store program in a variable
    size_t program_size;
    unsigned char* program;
    int resultRead = read_file_to_memory(file_path, &program, &program_size);
    
    if (resultRead != 0)
    {
        free(program);
        interpreter_free(&cpu);
        //TODO: print error

        return EXIT_FAILURE;
    }

    if (program_size > cpu.memory_size)
    {
        free(program);
        interpreter_free(&cpu);
        //TODO: print error

        return EXIT_FAILURE;
    }

    //Copy program to interpreter
    memcpy(cpu.memory, program, program_size);

    free(program);


    //Main loop
    //TODO: set up specific addresses for I/O, etc
    bool isRunning = true;
    while(isRunning)
    {
        interpreter_next(&cpu);
    }

    interpreter_free(&cpu);

    return EXIT_SUCCESS;
}
