#include "Parser/AstNode.h"

#include <memory>
#include <string>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

Parser::Parser(std::vector<TokenStruct> ts) : 
head(std::make_unique<Ast::Program>()),
ph(ts)
{}

void Parser::build(){
  head->match(ph);
}

Ast::Node* Parser::getAst(){
  return head.get();
};

std::vector<std::string> Parser::IndentDisplay() {
    std::vector<std::string> result;
    
    std::function<void(Ast::Node*, int)> dfs = [&](Ast::Node* node, int depth) {
        if (!node) return;
        
        // indent by depth so structure is visible
        result.push_back(std::string(depth * 2, ' ') + node->toString());
        
        for (auto& child : node->children) {
            dfs(child.get(), depth + 1);
        }
    };
    
    dfs(head.get(), 0);
    return result;
}

std::string Parser::flatDisplay() {
    std::string result;
    
    std::function<void(Ast::Node*)> dfs = [&](Ast::Node* node) {
        if (!node) return;
        
        bool isLeaf = node->children.empty();
        
        if (isLeaf) {
            result += node->toString();
        } else {
            result += "(" + node->toString();
            for (auto& child : node->children) {
                result += " ";
                dfs(child.get());
            }
            result += ")";
        }
    };
    
    dfs(head.get());
    return result;
}


/***
Parser Head
***/


ParserHead::ParserHead(std::vector<TokenStruct> TokenVector)
    : tokens(std::move(TokenVector)) {
  
  nextToken();
}

void ParserHead::nextToken() {
  if (pos >= tokens.size()) {
    curTkn = TokenStruct{TknType::END_F, "", 0, 0};
  } else {
    curTkn = std::move(tokens.at(pos++));
  }
}

/***
Non-Terminals
***/

Ast::Terminal::Terminal(std::string v) { this->value = std::move(v); }
std::string Ast::Terminal::toString() { return value; };
void Ast::Terminal::match(ParserHead& /*ph*/) {};

/* ****************************

CORE STATEMENT SEQUENCE

**************************** */

// Program
STR_SET(Program)
void Ast::Program::match(ParserHead& ph) {
  GUARD();
  auto stat = NT_CONVERT(Statement);
  stat->match(ph);
  GUARD();
  children.push_back(std::move(stat));
  expect(ph, TknType::END_F);
}

// Statement
STR_SET(Statement)
void Ast::Statement::match(ParserHead& ph) {
  GUARD();

  while (ph.curTkn.type == TknType::LET || ph.curTkn.type == TknType::RETURN) {
    if (ph.curTkn.type == TknType::LET) {
      auto let = NT_CONVERT(LetStatement);
      let->match(ph);
      GUARD();
      children.push_back(std::move(let));
    } else {
      auto ret = NT_CONVERT(ReturnStatement);
      ret->match(ph);
      GUARD();
      children.push_back(std::move(ret));
    }
  }
}

STR_SET(LetStatement)
void Ast::LetStatement::match(ParserHead& ph) {
  GUARD();

  expect(ph, TknType::LET);

  auto ident = T_CONVERT(ph.curTkn.literal);
  expect(ph, TknType::IDENT);
  expect(ph, TknType::ASSIGN);

  auto expr = NT_CONVERT(Expression);
  expr->match(ph);
  GUARD();

  expect(ph, TknType::SEMICOLON);

  children.push_back(std::move(ident));
  children.push_back(std::move(expr));
}

STR_SET(ReturnStatement)
void Ast::ReturnStatement::match(ParserHead& ph) {
  GUARD();
  expect(ph, TknType::RETURN);

  auto formula = NT_CONVERT(Formula);
  formula->match(ph);
  GUARD();

  expect(ph, TknType::SEMICOLON);
  children.push_back(std::move(formula));
}

STR_SET(Expression)
void Ast::Expression::match(ParserHead& ph) {
  GUARD();

  GET_TOKENS();

  if (CMP_CUR(TknType::FUNC)) {
    auto func_def = NT_CONVERT(FuncDef);
    func_def->match(ph);
    GUARD();
    children.push_back(std::move(func_def));
  } else if (CMP_CUR(TknType::IDENT) || CMP_CUR(TknType::INT) ||
             CMP_CUR(TknType::LPAREN)) {
    auto formula = NT_CONVERT(Formula);
    formula->match(ph);
    GUARD();
    children.push_back(std::move(formula));
  } else {
    ph.addError(cur, UNEXPECTED_TKN_ERR);
  };
};

/* ****************************
   OPERANDS AND OPERATORS
**************************** */

// FORMULA -> ADD_EXPR
STR_SET(Formula)
void Ast::Formula::match(ParserHead& ph) {
  GUARD();
  auto add_expr = NT_CONVERT(AddExpr);
  add_expr->match(ph);
  GUARD();
  children.push_back(std::move(add_expr));
}

// ADD_EXPR -> MUL_EXPR ADD_EXPR'
// ADD_EXPR' -> (+|-) MUL_EXPR ADD_EXPR' | E
STR_SET(AddExpr)
void Ast::AddExpr::match(ParserHead& ph) {
  GUARD();

  auto mul = NT_CONVERT(MulExpr);
  mul->match(ph);
  GUARD();
  children.push_back(std::move(mul));

  GET_TOKENS();
  if (CMP_CUR(TknType::PLUS) || CMP_CUR(TknType::MINUS)) {
    auto add_expr2 = NT_CONVERT(AddExpr2);
    add_expr2->match(ph);
    GUARD();
    children.push_back(std::move(add_expr2));
  }
}

STR_SET(AddExpr2)
void Ast::AddExpr2::match(ParserHead& ph) {
  auto op = T_CONVERT(ph.curTkn.literal);
  ph.nextToken();

  auto next_mul = NT_CONVERT(MulExpr);
  next_mul->match(ph);
  GUARD();

  children.push_back(std::move(op));
  children.push_back(std::move(next_mul));

  GET_TOKENS();
  if (CMP_CUR(TknType::PLUS) || CMP_CUR(TknType::MINUS)) {
    auto add_expr2 = NT_CONVERT(AddExpr2);
    add_expr2->match(ph);
    GUARD();
    children.push_back(std::move(add_expr2));
  }
}

// MUL_EXPR -> CALL MUL_EXPR'
// MUL_EXPR' -> (*|/) CALL MUL_EXPR' | E
STR_SET(MulExpr)
void Ast::MulExpr::match(ParserHead& ph) {
  GUARD();

  auto call = NT_CONVERT(Call);
  call->match(ph);
  GUARD();
  children.push_back(std::move(call));

  GET_TOKENS();
  // MUL_EXPR' as a recursion
  if (CMP_CUR(TknType::ASTERISK) || CMP_CUR(TknType::SLASH)) {
    auto next_call = NT_CONVERT(MulExpr2);
    next_call->match(ph);
    GUARD();
    children.push_back(std::move(next_call));
  }
}

STR_SET(MulExpr2)
void Ast::MulExpr2::match(ParserHead& ph) {
  GUARD();

  auto op = T_CONVERT(ph.curTkn.literal);
  ph.nextToken();

  auto call = NT_CONVERT(Call);
  call->match(ph);
  GUARD();

  children.push_back(std::move(op));
  children.push_back(std::move(call));

  GET_TOKENS();
  if (CMP_CUR(TknType::ASTERISK) || CMP_CUR(TknType::SLASH)) {
    auto next_call = NT_CONVERT(MulExpr2);
    next_call->match(ph);
    GUARD();
    children.push_back(std::move(next_call));
  }
}

// CALL -> IDENT CALL' | NUMBER | ( FORMULA )
// CALL' -> ( ARG ) | E
STR_SET(Call)
void Ast::Call::match(ParserHead& ph) {
  GUARD();
  GET_TOKENS();

  if (CMP_CUR(TknType::IDENT)) {
    auto ident = T_CONVERT(ph.curTkn.literal);
    ph.nextToken();

    // CALL' -> ( ARG ) | E
    if (ph.curTkn.type == TknType::LPAREN) {
      ph.nextToken();  // consume (

      auto args = NT_CONVERT(Arg);
      args->match(ph);
      GUARD();

      expect(ph, TknType::RPAREN);

      children.push_back(std::move(ident));
      children.push_back(std::move(args));
    } else {
      // just an identifier, no call
      children.push_back(std::move(ident));
    }
  } else if (CMP_CUR(TknType::INT)) {
    auto num = T_CONVERT(ph.curTkn.literal);
    expect(ph, TknType::INT);
    children.push_back(std::move(num));
  } else if (CMP_CUR(TknType::LPAREN)) {
    expect(ph, TknType::LPAREN);  // consume (

    auto formula = NT_CONVERT(Formula);
    formula->match(ph);
    GUARD();

    expect(ph, TknType::RPAREN);
    children.push_back(std::move(formula));
  } else {
    ph.addError(cur, UNEXPECTED_TKN_ERR);
  }
}

/* ****************************
   FUNCTION INPUTS
**************************** */

// ARG -> E | FORMULA ARG'
// ARG' -> , FORMULA ARG' | E
STR_SET(Arg)
void Ast::Arg::match(ParserHead& ph) {
  GET_TOKENS();
  while (!(CMP_CUR(TknType::RPAREN)) && !ph.hasError()) {
    auto formula = NT_CONVERT(Formula);
    formula->match(ph);
    GUARD();
    children.push_back(std::move(formula));
    GET_TOKENS();
    if (CMP_CUR(TknType::COMMA)) {
      ph.nextToken();  // consume ,
    } else {
      break;
    }
  }
}

// PARAMS_DEF -> E | IDENT PARAMS_DEF'
// PARAMS_DEF' -> , IDENT PARAMS_DEF' | E
STR_SET(ParamsDef)
void Ast::ParamsDef::match(ParserHead& ph) {
  GET_TOKENS();

  // E case — no params
  while (!(CMP_CUR(TknType::RPAREN)) && !ph.hasError()) {
    auto ident = T_CONVERT(ph.curTkn.literal);
    expect(ph, TknType::IDENT);
    children.push_back(std::move(ident));

    GET_TOKENS();
    if (CMP_CUR(TknType::COMMA)) {
      ph.nextToken();  // consume ,
    } else {
      break;
    }
  }
}

/* ****************************
   FUNCTION DEFINITION
**************************** */

// MULTI_STATEMENT -> STATEMENT MULTI_STATEMENT | E
STR_SET(MultiStatement)
void Ast::MultiStatement::match(ParserHead& ph) {
  GUARD();

  while (ph.curTkn.type != TknType::RBRACE &&
         ph.curTkn.type != TknType::END_F) {
    auto statement = NT_CONVERT(Statement);
    statement->match(ph);
    GUARD();
    children.push_back(std::move(statement));
  }
}

// FUNC_DEF -> fn ( PARAMS_DEF ) { MULTI_STATEMENT }
STR_SET(FuncDef)
void Ast::FuncDef::match(ParserHead& ph) {
  GUARD();

  expect(ph, TknType::FUNC);    // consume fn
  expect(ph, TknType::LPAREN);  // consume (

  auto params = NT_CONVERT(ParamsDef);
  params->match(ph);
  GUARD();

  expect(ph, TknType::RPAREN);  // consume )
  expect(ph, TknType::LBRACE);  // consume {

  auto body = NT_CONVERT(MultiStatement);
  body->match(ph);
  GUARD();

  expect(ph, TknType::RBRACE);  // consume }

  children.push_back(std::move(params));
  children.push_back(std::move(body));
}