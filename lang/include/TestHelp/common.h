#pragma once
#include <memory>
#include <vector>

#include "Lexer/Token.h"
#include "Parser/AstNode.h"
#include "Semantic/Object.h"

/**** Lexing Test infrastructure ****/

bool compareExepectedAndReality(std::vector<TokenStruct> expected,
                                std::string input);

std::vector<TokenStruct> lexInput(const std::string& input);

/****  Parser Test infrastructure ****/

bool compareExepectedAndRealityParser(const std::string& input,
                                      std::string expected_output);

bool compareExepectedAndRealityParserExpectingError(Parser& parser);

/**** Semantic Analyser Test infrastructure ****/

struct EvalTestHarness {
  std::vector<std::unique_ptr<ObjTypes::ObjectBase>> objects;
  std::string rawInput;
  Evaluator e;

  EvalTestHarness(
      const std::string& input,
      std::vector<std::unique_ptr<ObjTypes::ObjectBase>> expected_outputs)
      : rawInput(input),
        objects(std::move(expected_outputs)),
        e(Parser(lexInput(input))) {}

  const std::vector<std::unique_ptr<ObjTypes::ObjectBase>>& eval() const {
    objects.clear();
    return objects;
  }
};
