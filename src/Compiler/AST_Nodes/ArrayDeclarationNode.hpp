#pragma once

#include <string>
#include <vector>
#include "StatementNode.hpp"

struct ArrayDeclarationNode: StatementNode
{
    std::string name;
    size_t size;
    std::vector<int64_t> items;
};
