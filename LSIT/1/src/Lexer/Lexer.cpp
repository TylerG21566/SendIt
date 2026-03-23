#include "Lexer/Lexer.h"

#include <cctype>
#include <iostream>
#include <string>
#include <string_view>

#include "Lexer/Token.h"

Lexer::Lexer(std::string input)
    : input(std::move(input)),
      position(0),
      readPosition(0),
      size(this->input.size()),
      debug_mode(true),
      ls() {
  readChar();
  skipWhiteSpace();
}

void Lexer::skipWhiteSpace() {
  while (std::isspace(ch)) {
    readChar();
  };
};

void Lexer::printDebugVector() {
  if (!debug_mode) return;

  std::cout << "//////////////////////////" << std::endl << std::endl;

  for (auto t : tokens)
    std::cout << "Token_Type: " << t.type << " | "
              << "Token_literal: " << t.literal << std::endl;

  std::cout << "//////////////////////////" << std::endl << std::endl;
};

char Lexer::peek() {
  if (readPosition >= getInputSize()) return NULL_CH;

  return input.at(readPosition);
};

int Lexer::getInputSize() { return size; }

int Lexer::readChar() {
  if (readPosition >= getInputSize())
    ch = NULL_CH;
  else
    ch = input.at(readPosition);

  position = readPosition;
  readPosition++;
  return ch;
}

TokenStruct Lexer::NextToken() {
  TokenStruct tok;
  TokenType tt;

  std::string working_literal = "";
  bool new_token_needed = false;
  char firstCharacter = ch;

  ls = LexerState();
  ls.update_state(ch);

  while (!new_token_needed) {
    if (debug_mode)
      std::cout << position << " " << readPosition << " " << ch << " "
                << this->peek() << std::endl;

    new_token_needed = !ls.next_character_perserves_state(this->peek());
    working_literal += ch;
    readChar();
  }

  skipWhiteSpace();

  auto it = SINGLE_CHAR_TOKENS.find(firstCharacter);
  auto itk = KEYWORDS.find(working_literal);

  // get substring Token Type

  if (it != SINGLE_CHAR_TOKENS.end())
    tt = it->second;

  else if (firstCharacter == '!') {
    if (working_literal == TknType::NEQ)
      tt = TknType::NEQ;
    else
      tt = TknType::BANG;
  } else if (firstCharacter == '=') {
    if (working_literal == TknType::EQ)
      tt = TknType::EQ;
    else
      tt = TknType::ASSIGN;
  }

  else if (itk != KEYWORDS.end())
    tt = itk->second;

  else if (isdigit(working_literal.at(0)))
    tt = TknType::INT;
  else if (isalnum(working_literal.at(working_literal.size() - 1)))
    tt = TknType::IDENT;
  else if (working_literal == "!=")
    tt = TknType::NEQ;
  else if (working_literal == "==")
    tt = TknType::EQ;

  else
    tt = TknType::ILLEGAL;

  tok = newToken(tt, working_literal);

  if (debug_mode) tokens.push_back(tok);

  return tok;
}

TokenStruct Lexer::newToken(std::string_view tknType,
                            std::string previous_literal) {
  return TokenStruct{tknType, previous_literal};
};

Lexer sourceStringLexer(std::string input) { return Lexer(input); };