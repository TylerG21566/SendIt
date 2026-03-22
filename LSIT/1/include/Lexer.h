#pragma once
#include <string_view>
#include <string>
#include "Token/Token.h"

constexpr char NULL_CH = '\0';

class Lexer {
 private:
  std::string_view input;
  int position;
  int readPosition;  // always points to "next character"
  char ch;
  int size;

 public:
  Lexer(std::string input);

  int getInputSize();

  int readChar();

  TokenStruct NextToken();

  TokenStruct newToken(std::string_view tknType, char literal);
};

Lexer sourceStringLexer(std::string input);
