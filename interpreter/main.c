#include <stdbool.h>
#include <string.h>

#include "interpreter.h"
#include "utils.h"


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
    if (resultInitCpu != ERR_INTERPRETER_INIT_SUCCESS)
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
        int resultNext = interpreter_next(&cpu);
        
        if (resultNext != ERR_INTERPRETER_NEXT_SUCCESS)
        {
            interpreter_free(&cpu);
            //TODO: error

            return EXIT_FAILURE;
        }
    }

    interpreter_free(&cpu);

    return EXIT_SUCCESS;
}
