#pragma once
#include <string_view>
#include <string>
#include "Token/Token.h"
#include <vector>
#include "Token/States.h"

constexpr char NULL_CH = '\0';

class Lexer {
 private:
  std::string input;
  int position;
  int readPosition;  // always points to "next character"
  char ch;
  int size;
  bool debug_mode;
  std::vector<TokenStruct> tokens;
  LexerState ls;
  

 public:
  Lexer(std::string input);

  void printDebugVector();

  int getInputSize();

  int readChar();

  char peek();

  TokenStruct NextToken();

  TokenStruct newToken(TokenType tknType, std::string previous_literal);

  
};

Lexer sourceStringLexer(std::string input);
