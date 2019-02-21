#pragma once

#include <string>
#include "StatementNode.hpp"
#include "ElementNode.hpp"

struct ConstantDeclarationNode: StatementNode
{
    std::string name;
    ElementNode value;
};
