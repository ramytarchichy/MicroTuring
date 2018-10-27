#include <stdio.h>
#include <iostream>
#include <fstream>

#include "common.hpp"
#include "interpreter.hpp"

using namespace micro_turing;


int main(int argc, const char** argv)
{
    const std::vector<std::string> args = ModernArgs(argc, argv);

    //Settings
    bool safe = true;
    bool got_file = false;
    std::string file_path;

    //Get settings from args
    for (size_t i = 1; i <= args.size()-1; i++)
    {
        const std::string& arg = args[i];

        if (arg == "--fast")
        {
            safe = false;
        }
        else if (arg[1] != '-' && !got_file)
        {
            file_path = arg;
            got_file = true;
        }
    }

    //Read file
    interpreter it = interpreter();
    std::ifstream file(file_path);
    if (file)
    {
        do
        {
            mpq_class q;
            file >> q;
            it.memory.push_back(q);
        }
        while (!file.eof());
    }
    file.close();
    
    //Main loop
    //TODO: don't run forever, set up specific addresses for I/O, etc
    while(true)
    {
        it.step(safe);
    }
}
