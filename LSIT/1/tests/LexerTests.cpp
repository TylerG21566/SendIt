#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Tests/common.h"

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
  std::string input = "1a23";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::INT, "1"}, {TknType::IDENT, "a23"}, {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, invalid_variable_as_number) {
  std::string input = "let 1234 = 5;";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::LET, "let"}, {TknType::INT, "1234"},    {TknType::ASSIGN, "="},
      {TknType::INT, "5"},   {TknType::SEMICOLON, ";"}, {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, illegal_nospace) {
  std::string input = "^&";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::ILLEGAL, "^"}, {TknType::ILLEGAL, "&"}, {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, illegal_with_nospace) {
  std::string input = "^ &";
  std::vector<TokenStruct> expected_tokens = {
      {TknType::ILLEGAL, "^"}, {TknType::ILLEGAL, "&"}, {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, illegal_in_identifier) {
  std::string input = "s^c";
  std::vector<TokenStruct> expected_tokens = {{TknType::IDENT, "s"},
                                              {TknType::ILLEGAL, "^"},
                                              {TknType::IDENT, "c"},
                                              {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, illegal_at_start_of_identifier) {
  std::string input = "s ^c";
  std::vector<TokenStruct> expected_tokens = {{TknType::IDENT, "s"},
                                              {TknType::ILLEGAL, "^"},
                                              {TknType::IDENT, "c"},
                                              {TknType::END_F, ""}};

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, basic_program) {
  std::string input = R"(let five = 5;
let ten = 10;
let add = fn(x, y) {
x + y;
};
let result = add(five, ten);
!-/*5;
5 < 10 > 5;)";

  std::vector<TokenStruct> expected_tokens = {
      // let five = 5;
      {TknType::LET, "let"},
      {TknType::IDENT, "five"},
      {TknType::ASSIGN, "="},
      {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"},
      // let ten = 10;
      {TknType::LET, "let"},
      {TknType::IDENT, "ten"},
      {TknType::ASSIGN, "="},
      {TknType::INT, "10"},
      {TknType::SEMICOLON, ";"},
      // let add = fn(x, y) {
      {TknType::LET, "let"},
      {TknType::IDENT, "add"},
      {TknType::ASSIGN, "="},
      {TknType::FUNC, "fn"},
      {TknType::LPAREN, "("},
      {TknType::IDENT, "x"},
      {TknType::COMMA, ","},
      {TknType::IDENT, "y"},
      {TknType::RPAREN, ")"},
      {TknType::LBRACE, "{"},
      // x + y;
      {TknType::IDENT, "x"},
      {TknType::PLUS, "+"},
      {TknType::IDENT, "y"},
      {TknType::SEMICOLON, ";"},
      // };
      {TknType::RBRACE, "}"},
      {TknType::SEMICOLON, ";"},
      // let result = add(five, ten);
      {TknType::LET, "let"},
      {TknType::IDENT, "result"},
      {TknType::ASSIGN, "="},
      {TknType::IDENT, "add"},
      {TknType::LPAREN, "("},
      {TknType::IDENT, "five"},
      {TknType::COMMA, ","},
      {TknType::IDENT, "ten"},
      {TknType::RPAREN, ")"},
      {TknType::SEMICOLON, ";"},
      // !-/*5;
      {TknType::BANG, "!"},
      {TknType::MINUS, "-"},
      {TknType::SLASH, "/"},
      {TknType::ASTERISK, "*"},
      {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"},
      // 5 < 10 > 5;
      {TknType::INT, "5"},
      {TknType::LT, "<"},
      {TknType::INT, "10"},
      {TknType::GT, ">"},
      {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"},
      // EOF
      {TknType::END_F, ""},
  };

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}

TEST(LexerTests, full_program) {
  std::string input = R"(let five = 5;
let ten = 10;
let add = fn(x, y) {
x + y;
};
let result = add(five, ten);
!-/*5;
5 < 10 > 5;
if (5 < 10) {
return true;
} else {
return false;
}
10 == 10;
10 != 9;)";

  std::vector<TokenStruct> expected_tokens = {
      // let five = 5;
      {TknType::LET, "let"},
      {TknType::IDENT, "five"},
      {TknType::ASSIGN, "="},
      {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"},
      // let ten = 10;
      {TknType::LET, "let"},
      {TknType::IDENT, "ten"},
      {TknType::ASSIGN, "="},
      {TknType::INT, "10"},
      {TknType::SEMICOLON, ";"},
      // let add = fn(x, y) {
      {TknType::LET, "let"},
      {TknType::IDENT, "add"},
      {TknType::ASSIGN, "="},
      {TknType::FUNC, "fn"},
      {TknType::LPAREN, "("},
      {TknType::IDENT, "x"},
      {TknType::COMMA, ","},
      {TknType::IDENT, "y"},
      {TknType::RPAREN, ")"},
      {TknType::LBRACE, "{"},
      // x + y;
      {TknType::IDENT, "x"},
      {TknType::PLUS, "+"},
      {TknType::IDENT, "y"},
      {TknType::SEMICOLON, ";"},
      // };
      {TknType::RBRACE, "}"},
      {TknType::SEMICOLON, ";"},
      // let result = add(five, ten);
      {TknType::LET, "let"},
      {TknType::IDENT, "result"},
      {TknType::ASSIGN, "="},
      {TknType::IDENT, "add"},
      {TknType::LPAREN, "("},
      {TknType::IDENT, "five"},
      {TknType::COMMA, ","},
      {TknType::IDENT, "ten"},
      {TknType::RPAREN, ")"},
      {TknType::SEMICOLON, ";"},
      // !-/*5;
      {TknType::BANG, "!"},
      {TknType::MINUS, "-"},
      {TknType::SLASH, "/"},
      {TknType::ASTERISK, "*"},
      {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"},
      // 5 < 10 > 5;
      {TknType::INT, "5"},
      {TknType::LT, "<"},
      {TknType::INT, "10"},
      {TknType::GT, ">"},
      {TknType::INT, "5"},
      {TknType::SEMICOLON, ";"},
      // if (5 < 10) {
      {TknType::IF, "if"},
      {TknType::LPAREN, "("},
      {TknType::INT, "5"},
      {TknType::LT, "<"},
      {TknType::INT, "10"},
      {TknType::RPAREN, ")"},
      {TknType::LBRACE, "{"},
      // return true;
      {TknType::RETURN, "return"},
      {TknType::IDENT, "true"},
      {TknType::SEMICOLON, ";"},
      // } else {
      {TknType::RBRACE, "}"},
      {TknType::ELSE, "else"},
      {TknType::LBRACE, "{"},
      // return false;
      {TknType::RETURN, "return"},
      {TknType::IDENT, "false"},
      {TknType::SEMICOLON, ";"},
      // }
      {TknType::RBRACE, "}"},
      // 10 == 10;
      {TknType::INT, "10"},
      {TknType::EQ, "=="},
      {TknType::INT, "10"},
      {TknType::SEMICOLON, ";"},
      // 10 != 9;
      {TknType::INT, "10"},
      {TknType::NEQ, "!="},
      {TknType::INT, "9"},
      {TknType::SEMICOLON, ";"},
      // EOF
      {TknType::END_F, ""},
  };

  ASSERT_TRUE(compareExepectedAndReality(expected_tokens, input));
}