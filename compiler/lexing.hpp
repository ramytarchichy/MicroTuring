#pragma once

#include <vector>
#include <string>

enum class token_type
{
    KEYWORD,
    COMMENT,
    WHITESPACE,
    NEWLINE,
    ID,
    NUMBER,
    OPERATOR,
    SYMBOL,
    UNKNOWN
};

class token
{
public:
    token();

    bool add(char c);

    std::string lexeme;
    token_type  type;

    size_t line_num;
    size_t char_num;
};

void tokenize(const std::string& text, std::vector<token>& tokens);
