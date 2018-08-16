#pragma once

#include <string>
#include "StatementNode.hpp"
#include "ElementNode.hpp"

struct VariableDeclarationNode: StatementNode
{
    std::string name;
    ElementNode value;
};
