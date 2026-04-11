#include "TestHelp/common.h"

#include <iostream>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/AstNode.h"

bool compareExepectedAndReality(std::vector<TokenStruct> expected_tokens,
                                std::string input) {
  Lexer lexer = sourceStringLexer(input, false);
  TokenStruct tkn = lexer.NextToken();
  int idx = 0;

  std::cout << input << std::endl;
  while (tkn.type != TknType::END_F) {
    // check tokens match
    if (idx == expected_tokens.size()) {
      lexer.printDebugVector();
      return false;
    }
    // if they dont assert
    if (expected_tokens.at(idx) != tkn) {
      lexer.printDebugVector();
      return false;
    }
    idx++;
    tkn = lexer.NextToken();
  };
  lexer.printDebugVector();
  std::cout << idx << " " << expected_tokens.size() << std::endl;
  return (idx + 1) == expected_tokens.size();
}

bool compareExepectedAndRealityParser(const std::string& input,
                                      std::string expected_output) {
  auto tokens = lexInput(input);
  Parser parser(tokens);

  parser.build();
  std::string actual = parser.flatDisplay();
  if (actual != expected_output) {
    std::cout << "Expected: " << expected_output << std::endl;
    std::cout << "Actual:   " << actual << std::endl;
  }
  return actual == expected_output;
}

// Helper to lex a string and return tokens
std::vector<TokenStruct> lexInput(const std::string& input) {
  Lexer lexer = sourceStringLexer(input, false);
  std::vector<TokenStruct> tokens;
  TokenStruct tkn = lexer.NextToken();
  while (tkn.type != TknType::END_F) {
    tokens.push_back(tkn);
    tkn = lexer.NextToken();
  }
  tokens.push_back(tkn);  // include END_F
  return tokens;
}

bool compareExepectedAndRealityParserExpectingError(Parser& parser) {
  parser.build();
  return parser.hasError();
};