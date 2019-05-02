#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "interpreter.h"
#include "utils.h"


#define LIMIT_MEMORY 64*1024 * sizeof(word_t)
#define LIMIT_SPEED  


int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        printf("ERROR: invalid number of arguments.\n");
        return EXIT_FAILURE;
    }

    //Settings
    const char* file_path = argv[1];


    //Store program in a variable
    size_t program_size;
    unsigned char* program;
    int result_read = read_file_to_memory(file_path, &program, &program_size);
    
    if (result_read != 0)
    {
        free(program);
        
        printf("ERROR: unable to read program file.\n");

        return EXIT_FAILURE;
    }

#ifdef LIMIT_MEMORY
    const size_t memory_limit = LIMIT_MEMORY;
#else
    size_t memory_limit = program_size;
#endif //LIMIT_MEMORY

    //Machine
    interpreter_t cpu;
    int result_init_cpu = interpreter_init(&cpu, memory_limit);
    if (result_init_cpu != ERR_INTERPRETER_INIT_SUCCESS)
    {
        printf("ERROR: unable to allocate memory.\n");
        return EXIT_FAILURE;
    }

#ifdef LIMIT_MEMORY

    if (program_size > cpu.memory_size)
    {
        free(program);
        interpreter_free(&cpu);
        
        printf("ERROR: program is too big.\n");

        return EXIT_FAILURE;
    }

#endif //LIMIT_MEMORY

    //Copy program to interpreter
    memcpy(cpu.memory, program, program_size);
    free(program);


    //Main loop
    bool is_running = true;
    while(is_running)
    {

#ifdef LIMIT_SPEED
        
        //Limit clock speed
        usleep(1);

#endif //LIMIT_SPEED

        // Execute next instruction
        int result_next = interpreter_next(&cpu);
        
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
