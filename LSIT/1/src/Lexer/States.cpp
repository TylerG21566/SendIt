#include "Token/States.h"

#include <cctype>

#include "Lexer.h"

bool LexerState::isAlpha(char ch) { return isalpha(ch); };

bool LexerState::isNum(char ch) { return isdigit(ch); };

void LexerState::update_state(char ch) {
  if (state == "") {
    switch (ch) {
      case '=':
        state = TknType::ASSIGN;
        break;
      case ';':
        state = TknType::SEMICOLON;
        break;
      case '(':
        state = TknType::LPAREN;
        break;
      case ')':
        state = TknType::RPAREN;
        break;
      case ',':
        state = TknType::COMMA;
        break;
      case '+':
        state = TknType::PLUS;
        break;
      case '{':
        state = TknType::LBRACE;
        break;
      case '}':
        state = TknType::RBRACE;
        break;
      case NULL_CH:
        state = TknType::END_F;
        break;
      default:
        if (LexerState::isNum(ch)) {
          state = TknType::INT;
        } else if (LexerState::isAlpha(ch)) {
          state = TknType::IDENT;
        } else {
          state = TknType::ILLEGAL;
        }
        break;
    };
  }
};

bool LexerState::next_character_perserves_state(char ch) {
  if (state != TknType::INT && state != TknType::IDENT) {
    return false;
  };

  if (std::isspace(ch)) {
    return false;
  };

  switch (ch) {
    case '=':
      return false;
    case ';':
      return false;
    case '(':
      return false;
    case ')':
      return false;
    case ',':
      return false;
    case '+':
      return false;
    case '{':
      return false;
    case '}':
      return false;
    case NULL_CH:
      return false;
    default:
      if (isalnum(ch)) return true;
      return false;
  };
};