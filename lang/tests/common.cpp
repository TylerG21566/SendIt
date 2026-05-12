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
  while (tkn.type != TknType::END_F) {
    if (idx >= expected_tokens.size()) {
      std::cerr << "TOO MANY TOKENS at idx " << idx << std::endl;
      return false;
    }
    std::cerr << "idx=" << idx
              << " expected_type=" << expected_tokens.at(idx).type
              << " expected_lit=" << expected_tokens.at(idx).literal
              << " actual_type=" << tkn.type
              << " actual_lit=" << tkn.literal << std::endl;
    if (expected_tokens.at(idx) != tkn) {
      std::cerr << "MISMATCH at idx " << idx << std::endl;
      return false;
    }
    idx++;
    tkn = lexer.NextToken();
  }
  std::cerr << "Loop ended. idx=" << idx
            << " expected_size=" << expected_tokens.size() << std::endl;
  return (idx + 1) == expected_tokens.size();
}

bool compareExepectedAndRealityParser(const std::string& input,
                                      std::string expected_output) {
  auto tokens = lexInput(input);
  for (auto t : tokens){
    std::cerr << t.type << " ___ " << t.literal << std::endl;
  }
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