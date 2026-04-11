#pragma once
#include <vector>

#include "Lexer/Token.h"
#include "Parser/AstNode.h"

bool compareExepectedAndReality(std::vector<TokenStruct> expected,
                                std::string input);

bool compareExepectedAndRealityParser(const std::string& input,
                                      std::string expected_output);

bool compareExepectedAndRealityParserExpectingError(Parser& parser);

std::vector<TokenStruct> lexInput(const std::string& input);