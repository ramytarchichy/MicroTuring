#include "lexing.hpp"
#include <ctype.h>
#include <unordered_set>
#include <ostream>

token::token()
{
    lexeme = "";
    type = token_type::UNKNOWN;

    line_num = 0;
    char_num = 0;
}


bool token::add(char c)
{
    // I could have done this with regular expressions, but I prefer having more control
    // over what's going on.


    //All the keywords used by the language go here
    const std::unordered_set<std::string> keywords = {
        "micro"
    };

    //All operators used by the language go here
    const std::unordered_set<std::string> operators = {
        ">",
        "[", "]"
    };

    //All syntax symbols used by the language go here
    const std::unordered_set<std::string> symbols = {
        ":", "."
    };


    switch (type)
    {
    case token_type::UNKNOWN:
        {
            lexeme += c;

            //Determine what type of token this is
            if (lexeme.size() == 1)
            {
                if (isdigit(lexeme[0])) type = token_type::NUMBER;
                else if (lexeme[0] == ';') type = token_type::COMMENT;
                else if (lexeme[0] == '\n') type = token_type::NEWLINE;
                else if (lexeme[0] == ' ' || lexeme[0] == '\t') type = token_type::WHITESPACE;

                if (type != token_type::UNKNOWN) return true;
            }
            if (operators.find(lexeme) != operators.end()) type = token_type::OPERATOR;
            else if (symbols.find(lexeme) != symbols.end()) type = token_type::SYMBOL;
            else if (isalnum(c))
            {
                //If it's in the set, it's a keyword. Otherwise it's an ID.
                if (keywords.find(lexeme) != keywords.end()) type = token_type::KEYWORD;
                else type = token_type::ID;
            }
            else if (type == token_type::UNKNOWN && (c == '\n' || c == ' ' || c == '\t'))
            {
                lexeme.pop_back();
                return false;
            }

            return true;
        }
        break;


    case token_type::KEYWORD:
    case token_type::ID:
        {
            //Make sure the character is alphanumeric. If so, add it to the string.
            if (isalnum(c)) lexeme += c;
            else return false;

            //If it's in the set above, it's a keyword. Otherwise it's an ID.
            if (keywords.find(lexeme) != keywords.end()) type = token_type::KEYWORD;
            else type = token_type::ID;

            return true;
        }
        break;


    case token_type::COMMENT:
        {
            //If it was determined to be a comment, it's always a comment until the end of
            //the line
            if (c == '\n') return false;
            lexeme += c;
            return true;
        }
        break;


    case token_type::NUMBER:
        {
            //No decimal/floating point numbers to deal with. Great success!
            if(!isdigit(c)) return false;
            lexeme += c;
            return true;
        }
        break;


    //Create a new token for each of these instead of appending more characters
    case token_type::NEWLINE:
    case token_type::WHITESPACE:
    case token_type::OPERATOR:
    case token_type::SYMBOL:
        {
            return false;
        }
        break;
    }
}


void tokenize(const std::string& text, std::vector<token>& tokens)
{
    token tk = token();
    
    size_t column_num = 1;
    size_t line_num = 1;

    for (char c : text)
    {
        while (!tk.add(c))
        {
            //The token is complete, add it to the list
            tokens.push_back(tk);

            //Start a new one
            tk = token();
            tk.line_num = line_num;
            tk.char_num = column_num;
        }

        //Keep track of line and column numbers, might be useful later for showing errors
        if (tk.type == token_type::NEWLINE)
        {
            column_num = 1;
            ++line_num;
        }
        else ++column_num;
    }

    //Add the last token
    if (tk.lexeme != "") tokens.push_back(tk);
}
