#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Tests/common.h"
#include "Token/Token.h"


TEST(LexerTests, Lexer_sanity_check_Test) {
  std::string input = "=+(){},;";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::ASSIGN, "="}, {TknType::PLUS, "+"},
      {TknType::LPAREN, "("}, {TknType::RPAREN, ")"},
      {TknType::LBRACE, "{"}, {TknType::RBRACE, "}"},
      {TknType::COMMA, ","},  {TknType::SEMICOLON, ";"},
      {TknType::END_F, "\0"}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, basic_source_code_with_white_space) {
  std::string input =
      "let five = 5;"
      "let ten =\n 10;\n"
      "\n\n\n\n\n"
      "let add = fn(x, y) {\n"
      "x + y;\n"
      "\t};\n"
      "\n"
      "let result = add(five, ten);";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::LET, "let"},     {TknType::IDENT, "five"},
      {TknType::ASSIGN, "="},    {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"}, {TknType::LET, "let"},
      {TknType::IDENT, "ten"},   {TknType::ASSIGN, "="},
      {TknType::INT, "10"},      {TknType::SEMICOLON, ";"},
      {TknType::LET, "let"},     {TknType::IDENT, "add"},
      {TknType::ASSIGN, "="},    {TknType::FUNC, "fn"},
      {TknType::LPAREN, "("},    {TknType::IDENT, "x"},
      {TknType::COMMA, ","},     {TknType::IDENT, "y"},
      {TknType::RPAREN, ")"},    {TknType::LBRACE, "{"},
      {TknType::IDENT, "x"},     {TknType::PLUS, "+"},
      {TknType::IDENT, "y"},     {TknType::SEMICOLON, ";"},
      {TknType::RBRACE, "}"},    {TknType::SEMICOLON, ";"},
      {TknType::LET, "let"},     {TknType::IDENT, "result"},
      {TknType::ASSIGN, "="},    {TknType::IDENT, "add"},
      {TknType::LPAREN, "("},    {TknType::IDENT, "five"},
      {TknType::COMMA, ","},     {TknType::IDENT, "ten"},
      {TknType::RPAREN, ")"},    {TknType::SEMICOLON, ";"},
      {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}


TEST(LexerTests, invalid_ident) {
  std::string input ="1a23";
  std::vector<TokenStruct> expected_tokens = {
    {TknType::INT, "1"}, {TknType::IDENT, "a23"},
    {TknType::END_F, ""}
  };

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, invalid_variable_as_number) {
  std::string input ="let 1234 = 5;";
  std::vector<TokenStruct> expected_tokens = {
    {TknType::LET, "let"}, {TknType::INT, "1234"},
    {TknType::ASSIGN, "="}, {TknType::INT, "5"},
    {TknType::SEMICOLON, ";"}, {TknType::END_F, ""}
  };

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}