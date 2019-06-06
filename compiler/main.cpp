#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "lexing.hpp"


int main(int argc, const char** argv)
{
    //Check arguments
    if (argc != 2)
    {
        std::cout << "Error: wrong number of parameters." << std::endl;
        return EXIT_FAILURE;
    }

    //Read file to memory (this is fine. after all, if the program is so large it can't
    //fit in memory, then how did you even write it? the editor would have crashed)
    std::ifstream file(argv[1], std::ios::ate);
    std::streamsize file_size = file.tellg();
    std::string program_text;
    program_text.resize(file_size);
    file.seekg(0, std::ios::beg);
    if (!file.read(&program_text[0], file_size))
    {
        std::cout << "Error: cannot read file." << std::endl;
        return EXIT_FAILURE;
    }

    //Tokenize
    std::vector<token> tokens = {};
    tokenize(program_text, tokens);

    //Print tokens
    for (token tk : tokens)
    {
        std::string type;
        switch (tk.type)
        {
                case token_type::COMMENT:
                type="COMMENT";
                break;

                case token_type::ID:
                type="ID";
                break;

                case token_type::KEYWORD:
                type="KEYWORD";
                break;

                case token_type::WHITESPACE:
                type="WHITESPACE";
                break;

                case token_type::NEWLINE:
                type="NEWLINE";
                break;

                case token_type::NUMBER:
                type="NUMBER";
                break;

                case token_type::OPERATOR:
                type="OPERATOR";
                break;

                case token_type::UNKNOWN:
                type="UNKNOWN";
                break;

                case token_type::SYMBOL:
                type="SYMBOL";
                break;
        }

        std::cout << type << " " << tk.lexeme << std::endl;
    }

    return EXIT_SUCCESS;
}
