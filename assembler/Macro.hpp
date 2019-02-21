#include "Token.hpp"

#include <string>
#include <vector>

struct Macro
{
    std::string name;
    std::vector<std::string> params;
    std::vector<std::vector<Token>> lines;
};
