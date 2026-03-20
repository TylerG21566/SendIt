#include <gtest/gtest.h>

#include <iostream>
#include <string_view>
#include <vector>

#include "Token/Token.h"

TEST(Lexer, Lexer_sanity_check_Test) {
  std::vector<TokenStruct> expected_tokens = {
      {TknType::ASSIGN, "="}, {TknType::PLUS, "+"},      {TknType::LPAREN, "("},
      {TknType::RPAREN, ")"}, {TknType::LBRACE, "{"},    {TknType::RBRACE, "}"},
      {TknType::COMMA, ","},  {TknType::SEMICOLON, ";"}, {TknType::END_F, ""}};

    
}