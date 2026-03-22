#pragma once
#include <string_view>
#include <string>
#include <unordered_map>

using TokenType = std::string_view;

namespace TknType {
  
  // CONTROL
  constexpr TokenType ILLEGAL = "ILLEGAL";
  constexpr TokenType END_F = "EOF";

  // ALPHA LITERAL: literal that starts with letter
  constexpr TokenType FUNC = "FUNC";
  constexpr TokenType LET = "LET";
  constexpr TokenType IDENT = "IDENT";

  // NUMERIC LITERAL: literal that starts with number
  constexpr TokenType INT = "INT";

  constexpr TokenType ASSIGN = "=";
  constexpr TokenType PLUS = "+";

  // DELIMMITER
  constexpr TokenType COMMA = ",";
  constexpr TokenType SEMICOLON = ";";
  constexpr TokenType LPAREN = "(";
  constexpr TokenType RPAREN = ")";
  constexpr TokenType LBRACE = "{";
  constexpr TokenType RBRACE = "}";
}

inline const std::unordered_map<std::string, TokenType> KEYWORDS = {
  {"let",    TknType::LET},
  {"fn",     TknType::FUNC},
};

class Token {
 private:
  TokenType type;
  TokenType literal;

 //public:
};

struct TokenStruct{
  TokenType type;
  std::string literal;

  auto operator<=>(const TokenStruct&) const = default;
};