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
  if (readPosition >= getInputSize()) {
    return NULL_CH;
  }
  return input.at(readPosition);
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
    if (debug_mode) {
      std::cout << position << " " << readPosition << " " << ch << " "
                << this->peek() << std::endl;
    };
    readChar();
  }

  // skip unnecessary whitespace to next character
  while (std::isspace(ch)) {
    readChar();
  }

  TokenType tt;

  switch (firstCharacter) {
    case ';':
      tt = TknType::SEMICOLON;
      break;
    case '(':
      tt = TknType::LPAREN;
      break;
    case ')':
      tt = TknType::RPAREN;
      break;
    case ',':
      tt = TknType::COMMA;
      break;
    case '+':
      tt = TknType::PLUS;
      break;
    case '{':
      tt = TknType::LBRACE;
      break;
    case '}':
      tt = TknType::RBRACE;
      break;
    case '*':
      tt = TknType::ASTERISK;
      break;
    case '/':
      tt = TknType::SLASH;
      break;
    case '-':
      tt = TknType::MINUS;
      break;
    case '<':
      tt = TknType::LT;
      break;
    case '>':
      tt = TknType::GT;
      break;
      // potential 2 character Token
    case '!':
      if (working_literal == TknType::NEQ){
        tt = TknType::NEQ;
      }else{
        tt = TknType::BANG;
      }
      break;
    case '=':
      if (working_literal == TknType::EQ){
        tt = TknType::EQ;
      }else{
        tt = TknType::ASSIGN;
      }
      break;

    case NULL_CH:
      tt = TknType::END_F;
      break;

    default:

      auto it = KEYWORDS.find(working_literal);
      if (it != KEYWORDS.end()) {
        std::string_view type =
            it->second;  // it->first is the key, it->second is the value
        tt = type;
        break;
      }

      else if (isdigit(working_literal.at(0))) {
        tt = TknType::INT;
        break;
      } else if (working_literal == "fn") {
        tt = TknType::FUNC;
        break;
      } else if (isalnum(working_literal.at(working_literal.size() - 1))) {
        tt = TknType::IDENT;
        break;
      } else if (working_literal == "!=") {
        tt = TknType::NEQ;
        break;
      } else if (working_literal == "==") {
        tt = TknType::EQ;
        break;
      }

      else {
        tt = TknType::ILLEGAL;
        break;
      }
  };
  tok = newToken(tt, working_literal);

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