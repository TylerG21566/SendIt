#pragma once
#include <string_view>

namespace TknType {
  
  constexpr std::string_view ILLEGAL = "ILLEGAL";
  constexpr std::string_view END_F = "EOF";

  constexpr std::string_view IDENT = "IDENT";
  constexpr std::string_view INT = "INT";

  constexpr std::string_view ASSIGN = "=";
  constexpr std::string_view PLUS = "+";

  constexpr std::string_view COMMA = ",";
  constexpr std::string_view SEMICOLON = ";";

  constexpr std::string_view LPAREN = "(";
  constexpr std::string_view RPAREN = ")";
  constexpr std::string_view LBRACE = "{";
  constexpr std::string_view RBRACE = "}";

  constexpr std::string_view FUNC = "FUNC";
  constexpr std::string_view LET = "LET";
}

class Token {
 private:
  std::string_view type;
  std::string_view literal;

 //public:
};

struct TokenStruct{
  std::string_view type;
  std::string_view literal;
};