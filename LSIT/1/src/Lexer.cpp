#include "Lexer.h"

#include <string>
#include <string_view>

#include "Token/Token.h"

Lexer::Lexer(std::string input) {
  this->input = input;
  this->ch = 0;
  this->readPosition = 0;
  this->position = 0;
  this->size = input.size();
};

int Lexer::getInputSize() { return size; }

int Lexer::readChar() {
  if (readPosition >= getInputSize()) {
    ch = NULL_CH;
  } else {
    ch = input.at(readPosition);
  }
  position = readPosition;
  readPosition++;
  return ch;
}

TokenStruct Lexer::NextToken() {
  TokenStruct tok;
  readChar();
  switch (ch) {
    case '=':
      tok = newToken(TknType::ASSIGN, ch);
      break;
    case ';':
      tok = newToken(TknType::SEMICOLON, ch);
      break;
    case '(':
      tok = newToken(TknType::LPAREN, ch);
      break;
    case ')':
      tok = newToken(TknType::RPAREN, ch);
      break;
    case ',':
      tok = newToken(TknType::COMMA, ch);
      break;
    case '+':
      tok = newToken(TknType::PLUS, ch);
      break;
    case '{':
      tok = newToken(TknType::LBRACE, ch);
      break;
    case '}':
      tok = newToken(TknType::RBRACE, ch);
      break;
    case NULL_CH:
      tok = newToken(TknType::END_F, '\0');
      break;
  }

  return tok;
}

TokenStruct Lexer::newToken(std::string_view tknType, char literal) {
  return TokenStruct{tknType, literal};
};

Lexer sourceStringLexer(std::string input) { return Lexer(input); };
