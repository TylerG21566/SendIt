#pragma once
#include <string>
#include <string_view>
#include "Token/Token.h"

class LexerState{
    public:
    TokenType state = "";

    void update_state(char ch);

    bool next_character_perserves_state(char ch);

    bool isNum(char ch);

    bool isAlpha(char ch);

};