#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

#define AST_NODE_IMPL              \
  std::string toString() override; \
  void match(ParserHead& ph) override;

#define NT_CONVERT(type) std::make_unique<Ast::type>()
#define T_CONVERT(literal) std::make_unique<Ast::Terminal>(literal)

#define CMP_CUR(TokType) cur.type == TokType

#define STR_SET(NT) \
  std::string Ast::NT::toString() { return "" #NT ""; }

#define GET_TOKENS()               \
  auto cur = ph.getCurrentToken();

#define NEXT_TOKENS()         \
  ph.nextToken();             \
  cur = ph.getCurrentToken();

#define GUARD() \
  if (ph.hasError()) return

enum ParsingErrorEnum {
  EXPECTED_TKN_ERR,  // These happen when the parser expects a specific token
                     // but gets something else. Be specific about what you
                     // expected and what you got

  UNEXPECTED_TKN_ERR,  // When the parser encounters something that doesn't
                       // match any rule

  UNCLOSED_DELIMMITER_ERR,  // "Expected ')' to close '(' opened at line 1, col
                            // 9"

  EOF_ERR,  // When input ends unexpectedly
};

struct ParsingError : TokenStruct {
  ParsingErrorEnum errorType;

  ParsingError(TokenStruct token, ParsingErrorEnum errEnum)
      : TokenStruct(std::move(token)), errorType(errEnum) {}
};

struct ParserHead {
  std::vector<TokenStruct> tokens;
  size_t pos = 0;
  std::vector<ParsingError> errors;
  TokenStruct curTkn;

  ParserHead(std::vector<TokenStruct> TokenVector);
  void nextToken();

  TokenStruct getCurrentToken() { return curTkn; }

  bool hasError() { return !(errors.empty()); }

  const std::vector<ParsingError>& getErrors() { return errors; }

  void addError(TokenStruct ts, ParsingErrorEnum pnum) {
    errors.push_back(ParsingError(std::move(ts), pnum));
  }
};

namespace Ast {

struct Node {
  std::vector<std::unique_ptr<Node>> childrenToBuild;
  std::vector<std::unique_ptr<Node>> children;
  std::string value;

  virtual ~Node() = default;
  virtual std::string toString() = 0;  // = 0 makes it pure virtual
  virtual void match(ParserHead& ph) = 0;

  bool isTerminal() const { return children.empty(); }

  void add(std::unique_ptr<Node> node) { childrenToBuild.push_back(std::move(node)); }

  void buildChildren(ParserHead& ph) {
    value = this->toString();
    for (auto& child : childrenToBuild) {
      child->match(ph);
      if (ph.hasError()) return;
    }
    children.insert(children.end(),
                    std::make_move_iterator(childrenToBuild.begin()),
                    std::make_move_iterator(childrenToBuild.end()));
    childrenToBuild.clear();
  }

  void expect(ParserHead& ph, std::string type) {
    if (ph.curTkn.type != type) {
      ph.addError(ph.curTkn, EXPECTED_TKN_ERR);
      return;
    }
    ph.nextToken();
  }
};

/*
Non-Terminals
*/

// CORE STATEMENT SEQUENCE
struct Program : Node {
  AST_NODE_IMPL
};
struct Statement : Node {
  AST_NODE_IMPL
};
struct LetStatement : Node {
  AST_NODE_IMPL
};
struct ReturnStatement : Node {
  AST_NODE_IMPL
};
struct Expression : Node {
  AST_NODE_IMPL
};

// OPERANDS AND OPERATORS
struct Ident : Node {
  AST_NODE_IMPL
};
struct Number : Node {
  AST_NODE_IMPL
};
struct Formula : Node {
  AST_NODE_IMPL
};
struct AddExpr : Node {
  AST_NODE_IMPL
};
struct AddExpr2 : Node {
  AST_NODE_IMPL
};
struct MulExpr : Node {
  AST_NODE_IMPL
};
struct MulExpr2 : Node {
  AST_NODE_IMPL
};
struct Unary : Node {
  AST_NODE_IMPL
};
struct Call : Node {
  AST_NODE_IMPL
};
struct Call2 : Node {
  AST_NODE_IMPL
};

// FUNCTION INPUTS
struct ParamsDef : Node {
  AST_NODE_IMPL
};
struct ParamsDef2 : Node {
  AST_NODE_IMPL
};
struct Arg : Node {
  AST_NODE_IMPL
};
struct Arg2 : Node {
  AST_NODE_IMPL
};

// FUNCTION DEFINITION
struct MultiStatement : Node {
  AST_NODE_IMPL
};
struct FuncDef : Node {
  AST_NODE_IMPL
};

// Terminal
struct Terminal : Node {
  Terminal(std::string value);
  AST_NODE_IMPL
};

};  // namespace Ast

class Parser {
  std::unique_ptr<Ast::Node> head;
  ParserHead ph;

 public:
  Parser(std::vector<TokenStruct> ts);
  void build();
  Ast::Node* getAst();
  std::vector<std::string> IndentDisplay();
  std::string flatDisplay();
  bool hasError(){return ph.hasError();};
  const std::vector<ParsingError>& getErrors() { return ph.getErrors(); }
  std::string displayErrors(){
    std::string s = "";
    for (auto e : getErrors()){
      s += e.errorType ;
    }
    return s;
  };
};