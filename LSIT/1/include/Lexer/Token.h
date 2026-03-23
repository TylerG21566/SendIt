#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

using TokenType = std::string_view;

namespace TknType {

// UTILITY
constexpr TokenType ILLEGAL = "ILLEGAL";
constexpr TokenType END_F = "EOF";

// ALPHA LITERAL: literal that starts with letter
constexpr TokenType FUNC = "FUNC";
constexpr TokenType LET = "LET";
constexpr TokenType IDENT = "IDENT";

// NUMERIC LITERAL: literal that starts with number
constexpr TokenType INT = "INT";

// CONTROL FLOW
constexpr TokenType IF = "IF";
constexpr TokenType ELSE = "ELSE";
constexpr TokenType RETURN = "RETURN";

// OPERATORS
constexpr TokenType ASSIGN = "=";

constexpr TokenType PLUS = "+";
constexpr TokenType SLASH = "/";
constexpr TokenType ASTERISK = "*";
constexpr TokenType MINUS = "-";

constexpr TokenType GT = ">";
constexpr TokenType LT = "<";
constexpr TokenType BANG = "!";

constexpr TokenType NEQ = "!=";
constexpr TokenType EQ = "==";
// constexpr TokenType GTE = ">=";
// constexpr TokenType LTE = "<=";

// DELIMMITER
constexpr TokenType COMMA = ",";
constexpr TokenType SEMICOLON = ";";
constexpr TokenType LPAREN = "(";
constexpr TokenType RPAREN = ")";
constexpr TokenType LBRACE = "{";
constexpr TokenType RBRACE = "}";
}  // namespace TknType

inline const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"let", TknType::LET},
    {"fn", TknType::FUNC},
    {"if", TknType::IF},
    {"else", TknType::ELSE},
    {"return", TknType::RETURN},
};

inline const std::unordered_map<char, TokenType> SINGLE_CHAR_TOKENS = {
    {';', TknType::SEMICOLON}, {'(', TknType::LPAREN},   {')', TknType::RPAREN},
    {',', TknType::COMMA},     {'+', TknType::PLUS},     {'{', TknType::LBRACE},
    {'}', TknType::RBRACE},    {'*', TknType::ASTERISK}, {'/', TknType::SLASH},
    {'-', TknType::MINUS},     {'<', TknType::LT},       {'>', TknType::GT},
    {'\0', TknType::END_F},
};

struct TokenStruct {
  TokenType type;
  std::string literal;

  auto operator<=>(const TokenStruct&) const = default;
};