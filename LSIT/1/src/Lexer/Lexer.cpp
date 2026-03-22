#include "Lexer.h"

#include <cctype>
#include <iostream>
#include <string>
#include <string_view>

#include "Token/Token.h"

Lexer::Lexer(std::string input) {
  this->input = input;
  readPosition = 0;
  position = 0;
  size = input.size();
  ls = LexerState();
  readChar();

  debug_mode = true;
};

void Lexer::printDebugVector() {
  if (!debug_mode) return;

  std::cout << "//////////////////////////" << std::endl << std::endl;
  for (auto t : tokens) {
    std::cout << "Token_Type: " << t.type << " | "
              << "Token_literal: " << t.literal << std::endl;
  };

  std::cout << "//////////////////////////" << std::endl << std::endl;
};

char Lexer::peek() {
  char c;
  if (readPosition >= getInputSize()) {
    c = NULL_CH;
  } else {
    c = input.at(readPosition);
  }

  return c;
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

  std::string working_literal = "";
  bool new_token_needed = false;
  char firstCharacter = ch;

  ls = LexerState();

  while (!new_token_needed) {
    working_literal += ch;
    // update_state
    ls.update_state(ch);
    // get next_state
    new_token_needed = !ls.next_character_perserves_state(this->peek());
    std::cout << position << " " << readPosition << " " << ch << " " << this->peek() << std::endl;
    readChar();
  }

  // skip unnecessary whitespace to next character
  while (std::isspace(ch)) {
    readChar();
  }

  // std::cout << "INNNNNNNPPPPUUUUUTTT" << input << std::endl;

  switch (firstCharacter) {
    case '=':
      tok = newToken(TknType::ASSIGN, working_literal);
      break;
    case ';':
      tok = newToken(TknType::SEMICOLON, working_literal);
      break;
    case '(':
      tok = newToken(TknType::LPAREN, working_literal);
      break;
    case ')':
      tok = newToken(TknType::RPAREN, working_literal);
      break;
    case ',':
      tok = newToken(TknType::COMMA, working_literal);
      break;
    case '+':
      tok = newToken(TknType::PLUS, working_literal);
      break;
    case '{':
      tok = newToken(TknType::LBRACE, working_literal);
      break;
    case '}':
      tok = newToken(TknType::RBRACE, working_literal);
      break;
    case NULL_CH:
      tok = newToken(TknType::END_F, working_literal);
      break;
    default:
      auto it = KEYWORDS.find(working_literal);
      if (it != KEYWORDS.end()) {
          std::string_view type = it->second;   // it->first is the key, it->second is the value
          tok = newToken(type, working_literal);
          break;
      }
      else if (isdigit(working_literal.at(0))) {
        tok = newToken(TknType::INT, working_literal);
        break;
      } else if (working_literal == "fn") {
        tok = newToken(TknType::FUNC, working_literal);
        break;
      } else if (isalnum(working_literal.at(working_literal.size()-1))) {
        tok = newToken(TknType::IDENT, working_literal);
        break;
      } else {
        tok = newToken(TknType::ILLEGAL, working_literal);
        break; 
      }
  };

  if (debug_mode) {
    tokens.push_back(tok);
  }

  return tok;
}

TokenStruct Lexer::newToken(std::string_view tknType,
                            std::string previous_literal) {
  return TokenStruct{tknType, previous_literal};
};

Lexer sourceStringLexer(std::string input) { return Lexer(input); };