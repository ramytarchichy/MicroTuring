#include "Interpreter.hpp"
#include "common.hpp"

#include <iostream>
#include <fstream>

using namespace MicroTuring;


int main(int argc, const char **argv)
{
    const std::vector<std::string> args = ModernArgs(argc, argv);

    //The interpreter only takes 2 arguments, the program name and the file which contains the memory instructions.
    if (args.size()-1 != 1)
    {
        throw std::runtime_error("Invalid arguments");
    }

    //Parse file
    std::ifstream file(args[1], std::ios::binary);
    Interpreter interpreter = Interpreter();

    while(!file.eof())
    {
        int64_t myint;
        file.read(reinterpret_cast<char*>(&myint), sizeof(myint));
        
        if(file.eof()) break;

        interpreter.memory.push_back(myint);
    }

    file.close();

    //Interpret code
    interpreter.Run();

    //Print memory to termnal
    std::cout << "Accumulator: " << interpreter.acc << std::endl;
    std::cout << "Address:     " << interpreter.address << std::endl;
    for (int64_t i = 0; i < interpreter.memory.size(); i++)
    {
        std::cout << i << ": " << interpreter.memory[i] << std::endl;
    }
}
