#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Token/Token.h"
#include "Tests/common.h"

TEST(Lexer, Lexer_sanity_check_Test) {
  std::string input = "=+(){},;";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::ASSIGN, '='}, {TknType::PLUS, '+'},
      {TknType::LPAREN, '('}, {TknType::RPAREN, ')'},
      {TknType::LBRACE, '{'}, {TknType::RBRACE, '}'},
      {TknType::COMMA, ','},  {TknType::SEMICOLON, ';'},
      {TknType::END_F, '\0'}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens,input));
}