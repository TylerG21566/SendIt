#include "Token/States.h"

#include <cctype>

#include "Lexer.h"

bool LexerState::isAlpha(char ch) { return isalpha(ch); };

bool LexerState::isNum(char ch) { return isdigit(ch); };

void LexerState::update_state(char ch) {
  if (state == "") {
    switch (ch) {
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
      case '*':
        state = TknType::ASTERISK;
        break;
      case '/':
        state = TknType::SLASH;
        break;
      case '-':
        state = TknType::MINUS;
        break;
      case '<':
        state = TknType::LT;
        break;
      case '>':
        state = TknType::GT;
        break;
      // potential 2 character Token
      case '!':
        state = TknType::BANG;
        break;
      case '=':
        state = TknType::ASSIGN;
        break;

      // NULL terminator
      case NULL_CH:
        state = TknType::END_F;
        break;

      // ALPHANUMERIC SPOTTED
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

/* decide whether next character forces a token to be emmitted */
bool LexerState::next_character_perserves_state(char ch) {
  // WHITESPACE always emmits token
  // gauranteed 1 character length Token match
  if (std::isspace(ch) ||
      (state != TknType::INT && state != TknType::IDENT &&
       state != TknType::BANG && state != TknType::ASSIGN)) {
    return false;
  };

  // INT type
  if (state == TknType::INT) {
    return isdigit(ch);
  }

  if (state == TknType::IDENT) {
    return isalnum(ch);
  }

  if (state == TknType::BANG) {
    return ch == '=';
  }

  if (state == TknType::ASSIGN) {
    return ch == '=';
  }



  // ILLEGAL character
  return false;
};