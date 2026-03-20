#pragma once
#include <string_view>

class Lexer{
    private:
        std::string_view input;
        int position;
        int readPosition; // always points to "next character"
        char ch;

    public:
        Lexer(std::string_view input){
            this->input = input;
            this->ch = 0;
            this->readPosition = 0;
            this->position = 0;
        };

        int getInputSize(){
            return input.size();
        }
};

std::unique_ptr<Lexer> convertSourceString(std::string_view input){
    return std::make_unique<Lexer>(input);
};
