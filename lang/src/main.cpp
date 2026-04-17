#include <poll.h>

#include <format>
#include <iostream>
#include <string>
#include <string_view>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/AstNode.h"


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
    input += line + "\n";
    std::cout << "Got:\n" << input << std::endl;
    

    // Generate Lexed output
    Lexer lexer = sourceStringLexer(input, false);
    TokenStruct tkn = lexer.NextToken();
    std::vector<TokenStruct> tokens;

    while (tkn.type != TknType::END_F) {
      tokens.push_back(tkn);
      std::string msg =
          "(type, literal): ( " + tkn.type + ", " + tkn.literal + " )";
      std::cout << msg << std::endl;
      tkn = lexer.NextToken();
    }
    tokens.push_back(tkn);  // include END_F
    

    // Generate Parse Tree
    std::cout << std::endl << tokens.size() << "=========== Parse Tree ===========" << std::endl;
    
    Parser parser = Parser(tokens);
    parser.build();
    std::vector<ParsingError> errors = parser.getErrors();
    if (errors.size() > 0){
      for (auto err : errors){
        std::cout << "ERROR: " << parsingErrorEnumToString(err.errorType) <<  std::endl;
        std::cout << "error location: (row " << err.row << ", column " << err.col << ")" << std::endl;
      }
      
    } else{
      std::cout << parser.flatDisplay() << std::endl;
    }



    
  }
}
