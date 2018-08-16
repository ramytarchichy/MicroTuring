#pragma once

#include <string>
#include "ASTNode.hpp"

enum class SpecialAddress
{
    END,
    NEXT
};

struct SpecialAddressNode: ASTNode
{
    SpecialAddress value;
};
