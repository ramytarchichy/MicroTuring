#include "Token.hpp"

Token::Token(TokenType type, const std::string &text)
{
    this->type = type;
    this->text = text;
}

Token::Token() : Token(TokenType::UNKNOWN, "") {}
