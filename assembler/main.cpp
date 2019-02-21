/* 
 * This is my first ever compiler sort of thing, so cut me some slack if it
 * looks or (and) runs like garbage. I didn't use LLVM because it's overkill
 * for this and it's only compiling for one architecture. Also because no one's
 * using this. On the bright side, it's so ugly that it mimicks GCC perfectly.
 */

#include "Token.hpp"
#include "common.hpp"
#include "AST_Nodes/Nodes.hpp"

#include <unordered_set>

#include <sstream>
#include <iostream>
#include <fstream>

//Could have done this with regex, but I didn't. Why? I don't really know.
#pragma region Lexing


std::vector<Token> Lex(std::string input);

void PushToken(std::vector<Token> *tokens, Token *token)
{
    if (token->type == TokenType::IDENTIFIER && (token->text == "END" || token->text == ">"))
    {
        token->type = TokenType::KEYWORD;
    }
    else if (token->type == TokenType::FILE_INCLUDE)
    {
        std::ifstream file(token->text);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::vector<Token> lexed = Lex(buffer.str());
        tokens->insert(tokens->end(), lexed.begin(), lexed.end());
        return;
    }

    tokens->push_back(*token);
    *token = Token(TokenType::UNKNOWN, "");
}

std::vector<Token> Lex(std::string input)
{
    //Token stuff
    std::vector<Token> tokens = {};
    Token token = Token(TokenType::UNKNOWN, "");
    
    //Other state changes to keep track of
    int multilineDepth = 0;
    bool thisLineCommented = false;

    //Loop through the input characters
    size_t i = 0;
    size_t count_char = 1;
    size_t count_line = 1;

    for (char c:input)
    {
        switch (c)
        {
            //Macro
            case '!':
            {
                if (token.type == TokenType::COMMENT || token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
                else
                {
                    if (token.type != TokenType::UNKNOWN)
                    {
                        PushToken(&tokens, &token);
                    }

                    token.type = TokenType::MACRO_DECLARATION;
                }
            }

            //Array
            case '%':
            {
                if (token.type == TokenType::COMMENT || token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
                else
                {
                    if (token.type != TokenType::UNKNOWN)
                    {
                        PushToken(&tokens, &token);
                    }

                    token.type = TokenType::ARRAY_DECLARATION;
                }
            }
            break;
            
            //Constant
            case '*':
            {
                if (token.type == TokenType::COMMENT || token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
                else
                {
                    if (token.type != TokenType::UNKNOWN)
                    {
                        PushToken(&tokens, &token);
                    }

                    token.type = TokenType::CONSTANT_DECLARATION;
                }
            }

            //Variable
            case '$':
            {
                if (token.type == TokenType::COMMENT || token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
                else
                {
                    if (token.type != TokenType::UNKNOWN)
                    {
                        PushToken(&tokens, &token);
                    }

                    token.type = TokenType::VARIABLE_DECLARATION;
                }
            }

            //Address
            case ':':
            {
                if (token.type == TokenType::COMMENT || token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
                else
                {
                    if (token.type != TokenType::UNKNOWN)
                    {
                        PushToken(&tokens, &token);
                    }

                    token.type = TokenType::ADDRESS_DECLARATION;
                }
            }

            //Include
            case '@':
            {
                switch(token.type)
                {
                    case TokenType::UNKNOWN:
                    {
                        token.type = TokenType::FILE_INCLUDE;
                    }
                    break;

                    case TokenType::COMMENT:
                    case TokenType::FILE_INCLUDE:
                    {
                        token.text += c;
                    }
                    break;

                    default:
                    {
                        throw std::runtime_error("Unexpected \"" + std::string(1, c) + "\" at line " +
                            std::to_string(count_line) + ", column " + std::to_string(count_char) + ".");
                    }
                    break;
                }
            }
            break;

            //Comment
            case '#':
            {
                switch (token.type)
                {
                    default:
                        PushToken(&tokens, &token);
                    case TokenType::UNKNOWN:
                        token.type = TokenType::COMMENT;
                    case TokenType::COMMENT:
                    {
                        token.text += c;
                        if (i > 0)
                        {
                            if (input[i-1] == ']')
                            {
                                multilineDepth -= 1;
                                if (multilineDepth == 0 && !thisLineCommented)
                                {
                                    PushToken(&tokens, &token);
                                }
                                else if (multilineDepth < 0)
                                {
                                    throw std::runtime_error("Unexpected multiline comment closing tag at line "
                                        + std::to_string(count_line) + ", column " + std::to_string(count_char) + ".");
                                }
                            }
                            else if (multilineDepth == 0)
                            {
                                thisLineCommented = true;
                            }
                        }
                    }
                    break;
                }
            }
            break;
            case '[':
            {
                if (token.type == TokenType::COMMENT && input[i-1] == '#')
                {
                    multilineDepth++;
                    token.text += c;
                }
                else if (token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
            }
            break;

            //Array Access Operator (Yes, I chose `~` instead of `[]` because the latter seems like too much work for me)
            case '~':
            {
                switch (token.type)
                {
                    case TokenType::COMMENT:
                    case TokenType::FILE_INCLUDE:
                    {
                        token.text += c;
                    }
                    break;

                    default:
                        PushToken(&tokens, &token);
                    case TokenType::UNKNOWN:
                    {
                        token.type = TokenType::OPERATOR;
                        token.text += c;
                    }
                    break;
                }
            }

            //New Line
            case '\r':
            case '\n':
            {
                count_line++;
                count_char = 0;

                thisLineCommented = false;

                if (multilineDepth > 0)
                {
                    token.text += c;
                }
                else
                {
                    if (token.type != TokenType::UNKNOWN)
                    {
                        PushToken(&tokens, &token);
                    }

                    token.type = TokenType::SEPARATOR;
                    token.text += c;
                    PushToken(&tokens, &token);
                }
            }
            break;

            //Whitespace
            case '\t':
            case ' ':
            {
                if (token.type == TokenType::COMMENT || token.type == TokenType::FILE_INCLUDE)
                {
                    token.text += c;
                }
                else if (token.type == TokenType::WHITESPACE || token.text == "")
                {
                    token.type = TokenType::WHITESPACE;
                    token.text += c;
                }
                else
                {
                    PushToken(&tokens, &token);
                    token = Token(TokenType::WHITESPACE, std::string(1, c));
                }
            }
            break;

            //Anything else
            default:
            {
                switch (token.type)
                {
                    case TokenType::OPERATOR:
                    case TokenType::WHITESPACE:
                        PushToken(&tokens, &token);
                    case TokenType::UNKNOWN:
                    {
                        if (isdigit(c) || c == '+' || 'c' == '-')
                        {
                            token.type = TokenType::LITERAL;
                        }
                        else
                        {
                            token.type = TokenType::IDENTIFIER;
                        }
                    }
                    default:
                    {
                        token.text += c;
                    }
                    break;

                    case TokenType::LITERAL:
                    {
                        if (isdigit(c))
                        {
                            token.text += c;
                        }
                        else
                        {
                            throw std::runtime_error("Unexpected character \"" + std::string(1, c) + "\" in literal \""
                                + token.text + "\" at line " +std::to_string(count_line) + ", column " + std::to_string(count_char) + ".");
                        }
                    }
                    break;
                }
            }
            break;
        }

        //Increment index
        count_char++;
        i++;
    }

    PushToken(&tokens, &token);

    return tokens;
}


#pragma endregion

int main(int argc, const char **argv)
{
    const std::vector<std::string> args = ModernArgs(argc, argv);

    // Read file
    std::ifstream file(args[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();
    file.close();

    // Lexing (and File Inclusion)
    std::vector<Token> tokens = Lex(input);

    // Syntax Analysis
    std::vector<std::vector<Token>> lines = {};
    #pragma region

    size_t ln = 0;
    size_t macro_indent = 0;
    bool macro = false;

    std::vector<Token> line = {};

    for (Token t: tokens)
    {
        size_t indent = 0;
        size_t calculating_indent = true;

        switch (t.type)
        {
            //Use comments to increment line number, do not add to line
            case TokenType::COMMENT:
            {
                for (char c:t.text)
                {
                    switch (c)
                    {
                        case '\r':
                        case '\n':
                        {
                            ln++;
                        }
                        break;
                    }
                }
            }
            break;

            //Calculate indents, do not add to line
            case TokenType::WHITESPACE:
            {
                if (calculating_indent)
                {
                    for (char c:t.text)
                    {
                        switch (c)
                        {
                            case '\t':
                            {
                                indent += 4;
                            }
                            break;

                            case ' ':
                            {
                                indent += 1;
                            }
                            break;
                        }
                    }
                }
            }
            break;

            default:
            {
                calculating_indent = false;
                line.push_back(t);
            }
            break;

            case TokenType::SEPARATOR:
            {
                //Make sure line is not empty
                if (line.size() > 0)
                {
                    if (macro && indent <= macro_indent)
                    {
                        macro = false;
                        lines.push_back({Token(TokenType::MACRO_END, "")});
                    }
                    
                    switch (line[0].type)
                    {



                        //Line can't start with a literal...
                        case TokenType::LITERAL:
                        {
                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Line cannot start with a literal");
                        }
                        break;



                        //...nor with a keyword...
                        case TokenType::KEYWORD:
                        {
                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Line cannot start with a keyword");
                        }
                        break;



                        //...nor with an operator
                        case TokenType::OPERATOR:
                        {
                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Line cannot start with an operator");
                        }
                        break;



                        case TokenType::MACRO_DECLARATION:
                        {
                            if (macro)
                            {
                                throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Nested macro definitions are not allowed");
                            }
                            else
                            {
                                macro = true;
                                macro_indent = indent;

                                std::unordered_set<std::string> param_names = {};

                                //Parameters must be identifiers...
                                for (Token t2: std::vector<Token>(line.begin()+1, line.end()))
                                {
                                    if (t2.type != TokenType::IDENTIFIER)
                                    {
                                        throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Unexpected token \"" + t2.text + "\" in macro definition parameters");
                                    }
                                    else
                                    {
                                        //...and unique
                                        if (param_names.find(t2.text) == param_names.end())
                                        {
                                            param_names.insert(t2.text);
                                        }
                                        else
                                        {
                                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Duplicate parameter name \"" + t2.text + "\" in macro definition parameters");
                                        }
                                    }
                                }
                            }
                        }
                        break;



                        case TokenType::ADDRESS_DECLARATION:
                        {
                            if (line.size() != 1)
                            {
                                throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Unexpected token in address declaration");
                            }
                        }
                        break;



                        case TokenType::VARIABLE_DECLARATION:
                        case TokenType::CONSTANT_DECLARATION:
                        {
                            if (line.size() != 2 || (line[1].type != TokenType::IDENTIFIER && line[1].type != TokenType::LITERAL))
                            {
                                throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Invalid definition");
                            }
                        }
                        break;



                        case TokenType::ARRAY_DECLARATION:
                        {
                            for (Token t2: std::vector<Token>(line.begin()+1, line.end()))
                            {
                                switch (t2.type)
                                {
                                    case TokenType::IDENTIFIER:
                                    case TokenType::LITERAL:
                                    break;

                                    default:
                                    {
                                        throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Unexpected token in array definition");
                                    }
                                    break;
                                }
                            }
                        }
                        break;



                        case TokenType::IDENTIFIER:
                        {
                            uint8_t stack_pos = 0;

                            for (Token t2: std::vector<Token>(line.begin()+1, line.end()))
                            {
                                switch (t2.type)
                                {
                                    case TokenType::KEYWORD:
                                    {
                                        if (stack_pos == 2)
                                        {
                                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Expected literal after array access operator, found keyword");
                                        }

                                        stack_pos = 0;
                                    }
                                    break;

                                    case TokenType::IDENTIFIER:
                                    {
                                        switch (stack_pos)
                                        {
                                            case 0:
                                            case 1:
                                            {
                                                stack_pos = 1;
                                            }
                                            break;

                                            case 2:
                                            {
                                                stack_pos = 0;
                                            }
                                            break;
                                        }
                                    }
                                    break;

                                    case TokenType::OPERATOR:
                                    {
                                        if (stack_pos != 1)
                                        {
                                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Unexpected operator");
                                        }
                                        
                                        stack_pos++;
                                    }
                                    break;

                                    case TokenType::LITERAL:
                                    {
                                        if (stack_pos != 2)
                                        {
                                            throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Unexpected literal");
                                        }

                                        stack_pos = 0;
                                    }
                                    break;

                                    default:
                                    {
                                        throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Unexpected token");
                                    }
                                    break;
                                }

                                if (stack_pos == 2)
                                {
                                    throw std::runtime_error("Syntax error at line " + std::to_string(ln) + ": Expected literal or identifier after array access operator, nothing found");
                                }
                            }
                        }
                        break;
                    }
                }
            }
            break;
        }

        //Reset and process next line
        lines.push_back(line);
        line = {};
        ln++;
    }
    
    #pragma endregion

    // Parsing to AST
    #pragma region

    std::vector<StatementNode> statements = {};
    for (auto line: lines)
    {
        
    }

    #pragma endregion

    // CANCELLED: Semantic Analysis (Type Checking)
    /* The original idea was to check each `i` command's `a` and `b` arguments: if `a` doesn't refer to data ($),
     * or `b` doesn't refer to an address (:), then throw an error. However, due to time constraints, as well as this
     * feeling like an arbitrary artificial limitation of the language with no practical use apart from debugging,
     * which won't happen anyway because who's writing stuff in this, I decided to not implement it.
     */

    // Bytecode Generation
    
}
