#pragma once

#include "VariableNode.hpp"
#include "ElementNode.hpp"

struct ArrayAccessOperatorNode: ElementNode
{
    VariableNode array;
    ElementNode index;
};
