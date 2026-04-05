#include "Tests/common.h"

#include <iostream>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

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