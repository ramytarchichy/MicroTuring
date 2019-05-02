#include "utils.h"


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
