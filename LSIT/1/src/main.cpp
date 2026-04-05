#include <poll.h>

#include <format>
#include <iostream>
#include <string>
#include <string_view>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

constexpr std::string_view Prompt = ">> ";

int main() {
  std::cout << "STARTING MONKEY..." << std::endl;
  std::cout << "INPUT: " << std::endl << std::endl;

  while (true) {
    // read until blank line
    std::cout << Prompt;
    std::string line, input;
    std::cout << ">> ";
    std::getline(std::cin, line);
    std::cout << "Got:\n" << input << std::endl;
    input += line + "\n";

    // Generate Lexed output
    Lexer lexer = sourceStringLexer(input, false);
    TokenStruct tkn = lexer.NextToken();

    while (tkn.type != TknType::END_F) {
      std::string msg =
          "(type, literal): ( " + tkn.type + ", " + tkn.literal + " )";
      std::cout << msg << std::endl;
      tkn = lexer.NextToken();
    }
  }
}
