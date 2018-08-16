#pragma once

#include <string>
#include <vector>
#include "StatementNode.hpp"
#include "ElementNode.hpp"

struct MacroNode: StatementNode
{
    std::string name;
    std::vector<ElementNode> args;
};
