#pragma once

#include <string>


enum class TokenType
{
    IDENTIFIER,
    KEYWORD,
    LITERAL,
    OPERATOR,
    SEPARATOR,
    COMMENT,
    WHITESPACE,
    UNKNOWN,
    ADDRESS_DECLARATION,
    VARIABLE_DECLARATION,
    ARRAY_DECLARATION,
    MACRO_DECLARATION,
    MACRO_END,
    CONSTANT_DECLARATION,
    FILE_INCLUDE
};

struct Token
{
    //Constructors
    Token();
    Token(TokenType type, const std::string &text);

    TokenType   type;
    std::string text;
};
