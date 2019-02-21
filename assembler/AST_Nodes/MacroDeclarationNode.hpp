#pragma once

#include <string>
#include <vector>
#include "StatementNode.hpp"

struct MacroDeclarationNode: StatementNode
{
    std::string name;
    std::vector<std::string> params;
    std::vector<StatementNode> content;
};
