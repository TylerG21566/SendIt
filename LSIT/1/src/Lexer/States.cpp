#include "Lexer/States.h"

#include <cctype>

#include "Lexer/Lexer.h"

void LexerState::update_state(char ch) {
  auto it = SINGLE_CHAR_TOKENS.find(ch);

  if (it != SINGLE_CHAR_TOKENS.end()) state = it->second;

  // POTENTIAL 2 Length Symbols
  else if (ch == '!')
    state = TknType::BANG;
  else if (ch == '=')
    state = TknType::ASSIGN;

  // ALPHANUMERIC SPOTTED
  else if (isdigit(ch))
    state = TknType::INT;
  else if (isalpha(ch))
    state = TknType::IDENT;
  else
    state = TknType::ILLEGAL;
};

/* decide whether next character forces a token to be emmitted */
bool LexerState::next_character_perserves_state(char ch) {
  auto it = SINGLE_CHAR_TOKENS.find(ch);
  // WHITESPACE always emmits token
  // gauranteed 1 character length Token match
  if (std::isspace(ch) || it != SINGLE_CHAR_TOKENS.end()) return false;

  if (state == TknType::INT) return isdigit(ch);

  if (state == TknType::IDENT) return isalnum(ch);

  if (state == TknType::BANG || state == TknType::ASSIGN) return ch == '=';

  // ILLEGAL character
  return false;
};