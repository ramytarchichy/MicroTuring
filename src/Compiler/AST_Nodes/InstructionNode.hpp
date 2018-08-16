#pragma once

#include "ElementNode.hpp"
#include "StatementNode.hpp"

struct InstructionNode: StatementNode
{
    ElementNode a;
    ElementNode b;
};
