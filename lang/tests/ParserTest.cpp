
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/AstNode.h"
#include "TestHelp/common.h"

// let x = 5;
TEST(ParserTests, Parser_let_integer_assignment) {
  std::string input = "let x = 5;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call "
      "5))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// let x = 1 + 2;
TEST(ParserTests, Parser_let_addition) {
  std::string input = "let x = 1 + 2;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 1)) "
      "(AddExpr2 + (MulExpr (Call 2)))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// let x = 1 * 2;
TEST(ParserTests, Parser_let_multiplication) {
  std::string input = "let x = 1 * 2;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 1) "
      "(MulExpr2 * (Call 2)))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// let x = foo(1, 2);
TEST(ParserTests, Parser_let_function_call) {
  std::string input = "let x = foo(1, 2);";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call foo "
      "(Arg (ValueExpression (Compare (OrExpr (AndExpr (NotExpr (AddExpr "
      "(MulExpr (Call 1)))))))) (ValueExpression (Compare (OrExpr (AndExpr "
      "(NotExpr (AddExpr (MulExpr (Call 2)))))))))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// let f = fn(x, y) { return x + y; };
TEST(ParserTests, Parser_let_function_definition) {
  std::string input = "let f = fn(x, y) { return x + y; };";
  std::string expected =
      "(Program (Statement (LetStatement f (Expression (FuncDef (ParamsDef "
      "x y) (MultiStatement (Statement (ReturnStatement (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call x)) "
      "(AddExpr2 + (MulExpr (Call y)))))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// return 42;
TEST(ParserTests, Parser_return_integer) {
  std::string input = "return 42;";
  std::string expected =
      "(Program (Statement (ReturnStatement (ValueExpression (Compare "
      "(OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 42)))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// return return;
TEST(ParserTests, Parser_return_return_error) {
  std::string input = "return return;";
  auto tokens = lexInput(input);
  Parser parser(tokens);
  EXPECT_TRUE(compareExepectedAndRealityParserExpectingError(parser))
      << "Errors:  " << parser.displayErrors() << std::endl;
}

// Multiple statements
TEST(ParserTests, Parser_multiple_let_statements) {
  std::string input = "let x = 1; let y = 2;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 1)))))))))) "
      "(LetStatement y (Expression (ValueExpression (Compare (OrExpr "
      "(AndExpr (NotExpr (AddExpr (MulExpr (Call 2))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Subtraction
TEST(ParserTests, Parser_let_subtraction) {
  std::string input = "let x = 5 - 3;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 5)) "
      "(AddExpr2 - (MulExpr (Call 3)))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Division
TEST(ParserTests, Parser_let_division) {
  std::string input = "let x = 10 / 2;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 10) "
      "(MulExpr2 / (Call 2)))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Operator precedence: mul before add
TEST(ParserTests, Parser_precedence_mul_add) {
  std::string input = "let x = 1 + 2 * 3;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 1)) "
      "(AddExpr2 + (MulExpr (Call 2) (MulExpr2 * (Call 3))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Parenthesized expression overriding precedence
TEST(ParserTests, Parser_parenthesized_expression) {
  std::string input = "let x = (1 + 2) * 3;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call "
      "(ValueExpression (Compare (OrExpr (AndExpr (NotExpr (AddExpr "
      "(MulExpr (Call 1)) (AddExpr2 + (MulExpr (Call 2)))))))))) "
      "(MulExpr2 * (Call 3)))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Chained addition: 1 + 2 + 3
TEST(ParserTests, Parser_chained_addition) {
  std::string input = "let x = 1 + 2 + 3;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 1)) "
      "(AddExpr2 + (MulExpr (Call 2)) (AddExpr2 + (MulExpr "
      "(Call 3))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Chained multiplication: 2 * 3 * 4
TEST(ParserTests, Parser_chained_multiplication) {
  std::string input = "let x = 2 * 3 * 4;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 2) "
      "(MulExpr2 * (Call 3) (MulExpr2 * (Call 4))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Assign one variable to another
TEST(ParserTests, Parser_let_ident_assignment) {
  std::string input = "let x = y;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call "
      "y))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// No-arg function definition
TEST(ParserTests, Parser_function_no_params) {
  std::string input = "let f = fn() { return 1; };";
  std::string expected =
      "(Program (Statement (LetStatement f (Expression (FuncDef ParamsDef "
      "(MultiStatement (Statement (ReturnStatement (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr "
      "(Call 1))))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// No-arg function call
TEST(ParserTests, Parser_call_no_args) {
  std::string input = "let x = foo();";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call "
      "foo Arg))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Nested function calls: foo(bar(1))
TEST(ParserTests, Parser_nested_function_call) {
  std::string input = "let x = foo(bar(1));";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call foo "
      "(Arg (ValueExpression (Compare (OrExpr (AndExpr (NotExpr (AddExpr "
      "(MulExpr (Call bar (Arg (ValueExpression (Compare (OrExpr (AndExpr "
      "(NotExpr (AddExpr (MulExpr (Call 1))))))))))))))))))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Expression in function argument: foo(1 + 2)
TEST(ParserTests, Parser_expression_in_arg) {
  std::string input = "let x = foo(1 + 2);";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call foo "
      "(Arg (ValueExpression (Compare (OrExpr (AndExpr (NotExpr (AddExpr "
      "(MulExpr (Call 1)) (AddExpr2 + (MulExpr "
      "(Call 2))))))))))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Return a formula with operators
TEST(ParserTests, Parser_return_expression) {
  std::string input = "return 1 + 2;";
  std::string expected =
      "(Program (Statement (ReturnStatement (ValueExpression (Compare "
      "(OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 1)) (AddExpr2 "
      "+ (MulExpr (Call 2))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Let + return together
TEST(ParserTests, Parser_let_then_return) {
  std::string input = "let x = 5; return x;";
  std::string expected =
      "(Program (Statement (LetStatement x (Expression (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call 5)))))))))) "
      "(ReturnStatement (ValueExpression (Compare (OrExpr (AndExpr "
      "(NotExpr (AddExpr (MulExpr (Call x)))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Function with body containing multiple statements
TEST(ParserTests, Parser_function_multi_statement_body) {
  std::string input = "let f = fn(x) { let y = 1; return x + y; };";
  std::string expected =
      "(Program (Statement (LetStatement f (Expression (FuncDef (ParamsDef "
      "x) (MultiStatement (Statement (LetStatement y (Expression "
      "(ValueExpression (Compare (OrExpr (AndExpr (NotExpr (AddExpr "
      "(MulExpr (Call 1)))))))))) (ReturnStatement (ValueExpression "
      "(Compare (OrExpr (AndExpr (NotExpr (AddExpr (MulExpr (Call x)) "
      "(AddExpr2 + (MulExpr (Call y)))))))))))))))))";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}

// Error: missing semicolon
TEST(ParserTests, Parser_error_missing_semicolon) {
  std::string input = "let x = 5";
  auto tokens = lexInput(input);
  Parser parser(tokens);
  EXPECT_TRUE(compareExepectedAndRealityParserExpectingError(parser))
      << "Errors: " << parser.displayErrors() << std::endl;
}

// Error: missing expression
TEST(ParserTests, Parser_error_missing_expression) {
  std::string input = "let x = ;";
  auto tokens = lexInput(input);
  Parser parser(tokens);
  EXPECT_TRUE(compareExepectedAndRealityParserExpectingError(parser))
      << "Errors: " << parser.displayErrors() << std::endl;
}

// Error: missing closing paren
TEST(ParserTests, Parser_error_missing_rparen) {
  std::string input = "let x = (1 + 2;";
  auto tokens = lexInput(input);
  Parser parser(tokens);
  EXPECT_TRUE(compareExepectedAndRealityParserExpectingError(parser))
      << "Errors: " << parser.displayErrors() << std::endl;
}

// Empty program
TEST(ParserTests, Parser_empty_program) {
  std::string input = "";
  std::string expected = "(Program Statement)";
  ASSERT_TRUE(compareExepectedAndRealityParser(input, expected));
}