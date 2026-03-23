#pragma once
#include <vector>

#include "Lexer/Token.h"

bool compareExepectedAndReality(std::vector<TokenStruct> expected,
                                std::string input);