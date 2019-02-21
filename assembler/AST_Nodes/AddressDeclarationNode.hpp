#pragma once

#include <string>
#include "StatementNode.hpp"

struct AddressDeclarationNode: StatementNode
{
    std::string name;
};
