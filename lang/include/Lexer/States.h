#pragma once
#include <string>
#include <string_view>

#include "Lexer/Token.h"

class LexerState {
 public:
  TokenType state = "";

  void update_state(char ch);

  bool next_character_perserves_state(char ch, int length);
};