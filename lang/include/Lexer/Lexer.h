#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "Lexer/States.h"
#include "Lexer/Token.h"

inline constexpr char NULL_CH = '\0';

class Lexer {
 private:
  std::string input;
  int position;
  int readPosition;  // always points to "next character"
  char ch;
  int size;
  bool debug_mode;
  std::vector<TokenStruct> tokens;
  uint64_t row;
  uint64_t col;
  LexerState ls;

 public:
  Lexer(std::string input, bool debug_mode_enabled);

  void printDebugVector();

  int getInputSize();

  int readChar();

  char peek();

  void skipWhiteSpace();

  TokenStruct NextToken();

  TokenStruct newToken(TokenType tknType, std::string previous_literal, uint64_t row, uint64_t col);
};

Lexer sourceStringLexer(std::string input, bool dm);
