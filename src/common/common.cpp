#include "common.hpp"

#include <gmpxx.h>

std::vector<std::string> ModernArgs(int argc, const char **argv)
{
    std::vector<std::string> args = {};
    for (int i = 0; i < argc; i++)
    {
        std::string arg (argv[i]);
        args.push_back(arg);
    }
    return args;
}
