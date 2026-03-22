#include <vector>

#include "Lexer.h"
#include "Token/Token.h"
#include "Tests/common.h"

bool compareExepectedAndReality(std::vector<TokenStruct> expected_tokens,
                                std::string input) {
  Lexer lexer = sourceStringLexer(input);
  TokenStruct tkn = lexer.NextToken();
  int idx = 0;
  while (tkn.type != TknType::END_F) {
    // check tokens match
    if (idx == expected_tokens.size()) {
      return false;
    }
    // if they dont assert
    if (expected_tokens.at(idx) != tkn) {
      return false;
    }
    idx++;
    tkn = lexer.NextToken();
  };

  return idx == expected_tokens.size();
}