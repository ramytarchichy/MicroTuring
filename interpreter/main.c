#include <stdbool.h>
#include <string.h>

#include "interpreter.h"
#include "utils.h"


int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        printf("ERROR: invalid number of arguments.\n");
        return EXIT_FAILURE;
    }

    //Settings
    char* file_path = argv[1];

    //Machine
    interpreter_t cpu;
    int resultInitCpu = interpreter_init(&cpu, 16 * 1024 * 1024);
    if (resultInitCpu != ERR_INTERPRETER_INIT_SUCCESS)
    {
        printf("ERROR: unable to allocate memory.\n");
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
        
        printf("ERROR: unable to read program file.\n");

        return EXIT_FAILURE;
    }

    if (program_size > cpu.memory_size)
    {
        free(program);
        interpreter_free(&cpu);
        
        printf("ERROR: program is too big.\n");

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
        
        // Error handling
        if (result_next != ERR_INTERPRETER_NEXT_SUCCESS)
        {
            interpreter_free(&cpu);
            
            switch (result_next)
            {
                case ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_ACCESS:
                    printf("ERROR: program tried to access an illegal address.\n");
                    break;

                case ERR_INTERPRETER_NEXT_OUT_OF_BOUNDS_INSTRUCTION:
                    printf("ERROR: program jumped to an illegal address.\n");
                    break;

                default:
                    printf("ERROR: unkown program error: %u.\n", result_next);
                    break;
            }

            return EXIT_FAILURE;
        }
    }

    interpreter_free(&cpu);

    return EXIT_SUCCESS;
}
