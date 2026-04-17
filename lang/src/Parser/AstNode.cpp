#include <memory>
#include <string>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/AstNode.h"

Parser::Parser(std::vector<TokenStruct> ts)
    : head(std::make_unique<Ast::Program>()), ph(ts) {}

void Parser::build() { head->match(ph); }

Ast::Node* Parser::getAst() { return head.get(); };

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

  while (ph.curTkn.type == TknType::LET || ph.curTkn.type == TknType::RETURN
  || ph.curTkn.type == TknType::IF) {
    if (ph.curTkn.type == TknType::LET) {
      auto let = NT_CONVERT(LetStatement);
      let->match(ph);
      GUARD();
      children.push_back(std::move(let));
    } 
    else if (ph.curTkn.type == TknType::IF) {
      auto fi = NT_CONVERT(If);
      fi->match(ph);
      GUARD();
      children.push_back(std::move(fi));
    }
    else {
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

  auto value_expr = NT_CONVERT(ValueExpression);
  value_expr->match(ph);
  GUARD();

  expect(ph, TknType::SEMICOLON);
  children.push_back(std::move(value_expr));
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
  } else if (isAtomic(ph.curTkn.type)) {
    auto value_expr = NT_CONVERT(ValueExpression);
    value_expr->match(ph);
    GUARD();
    children.push_back(std::move(value_expr));
  } else {
    ph.addError(cur, UNEXPECTED_TKN_ERR);
  };
};

STR_SET(ValueExpression)
void Ast::ValueExpression::match(ParserHead& ph) {
  GUARD();
  GET_TOKENS();

  auto compare = NT_CONVERT(Compare);
  compare->match(ph);
  GUARD();
  children.push_back(std::move(compare));
}

STR_SET(If)
void Ast::If::match(ParserHead& ph) {
  GUARD();
  GET_TOKENS();

  expect(ph, TknType::IF);      // consume if
  expect(ph, TknType::LPAREN);  // consume (

  // ValueExpression
  auto value_expr = NT_CONVERT(ValueExpression);
  value_expr->match(ph);
  GUARD();
  children.push_back(std::move(value_expr));

  expect(ph, TknType::RPAREN);  // consume )
  expect(ph, TknType::LBRACE);  // consume {

  // MULTI_STATEMENT
  auto multi_statement = NT_CONVERT(MultiStatement);
  multi_statement->match(ph);
  GUARD();
  children.push_back(std::move(multi_statement));

  expect(ph, TknType::RBRACE);  // consume }

  // Then Else
  if (CMP_CUR(TknType::ELSE)) {
    auto then_else = NT_CONVERT(ThenElse);
    then_else->match(ph);
    GUARD();
    children.push_back(std::move(then_else));
  }

  expect(ph, TknType::SEMICOLON);  // consume ;
}

STR_SET(ThenElse)
void Ast::ThenElse::match(ParserHead& ph) {
  GUARD();
  GET_TOKENS();

  expect(ph, TknType::ELSE);    // consume else
  expect(ph, TknType::LBRACE);  // consume {

  // MULTI_STATEMENT
  auto multi_statement = NT_CONVERT(MultiStatement);
  multi_statement->match(ph);
  GUARD();
  children.push_back(std::move(multi_statement));

  expect(ph, TknType::RBRACE);  // consume }
}

/* ****************************
   OPERANDS AND OPERATORS
**************************** */

// COMPARE  -> OR_EXPR COMPARE'
// COMPARE' -> (==|!=|>|<) OR_EXPR | E

STR_SET(Compare)
void Ast::Compare::match(ParserHead& ph) {
  // OR_EXPR
  auto or_expr = NT_CONVERT(OrExpr);
  or_expr->match(ph);
  GUARD();
  children.push_back(std::move(or_expr));

  if (isComparison(ph.curTkn.type)) {
    auto compare2 = NT_CONVERT(Compare2);
    compare2->match(ph);
    GUARD();
    children.push_back(std::move(compare2));
  }
}

STR_SET(Compare2)
void Ast::Compare2::match(ParserHead& ph) {
  // OR_EXPR
  if (!isComparison(ph.curTkn.type)) {
    // throw_error(ph);
    return;
  }

  auto op = T_CONVERT(ph.curTkn.literal);
  ph.nextToken();
  children.push_back(std::move(op));

  auto or_expr = NT_CONVERT(OrExpr);
  or_expr->match(ph);
  GUARD();
  children.push_back(std::move(or_expr));
}

// OR_EXPR  -> AND_EXPR OR_EXPR'
// OR_EXPR' -> or AND_EXPR OR_EXPR' | E
STR_SET(OrExpr)
void Ast::OrExpr::match(ParserHead& ph) {
  GET_TOKENS();
  GUARD();

  auto and_expr = NT_CONVERT(AndExpr);
  and_expr->match(ph);
  GUARD();
  children.push_back(std::move(and_expr));

  if (CMP_CUR(TknType::OR)) {
    auto or_expr2 = NT_CONVERT(OrExpr2);
    or_expr2->match(ph);
    GUARD();
    children.push_back(std::move(or_expr2));
  }
}

STR_SET(OrExpr2)
void Ast::OrExpr2::match(ParserHead& ph) {
  GET_TOKENS();
  GUARD();

  expect(ph, TknType::OR);

  auto and_expr = NT_CONVERT(AndExpr);
  and_expr->match(ph);
  GUARD();
  children.push_back(std::move(and_expr));

  if (CMP_CUR(TknType::OR)) {
    auto or_expr2 = NT_CONVERT(OrExpr2);
    or_expr2->match(ph);
    GUARD();
    children.push_back(std::move(or_expr2));
  }
}

// AND_EXPR  -> NOT_EXPR AND_EXPR'
// AND_EXPR' -> and NOT_EXPR AND_EXPR' | E
STR_SET(AndExpr)
void Ast::AndExpr::match(ParserHead& ph) {
  GET_TOKENS();
  GUARD();

  auto not_expr = NT_CONVERT(NotExpr);
  not_expr->match(ph);
  GUARD();
  children.push_back(std::move(not_expr));

  if (CMP_CUR(TknType::AND)) {
    auto and_expr2 = NT_CONVERT(OrExpr2);
    and_expr2->match(ph);
    GUARD();
    children.push_back(std::move(and_expr2));
  }
}

STR_SET(AndExpr2)
void Ast::AndExpr2::match(ParserHead& ph) {
  GET_TOKENS();
  GUARD();

  expect(ph, TknType::AND);

  auto not_expr = NT_CONVERT(NotExpr);
  not_expr->match(ph);
  GUARD();
  children.push_back(std::move(not_expr));

  if (CMP_CUR(TknType::AND)) {
    auto and_expr2 = NT_CONVERT(AndExpr2);
    and_expr2->match(ph);
    GUARD();
    children.push_back(std::move(and_expr2));
  }
}

// NOT_EXPR -> not NOT_EXPR | ADD_EXPR
STR_SET(NotExpr)
void Ast::NotExpr::match(ParserHead& ph) {
  GET_TOKENS();
  GUARD();

  if (CMP_CUR(TknType::NOT)) {
    if (CMP_CUR(TknType::NOT)) {
      auto op = T_CONVERT("not");
      ph.nextToken();
      auto inner = NT_CONVERT(NotExpr);
      inner->match(ph);
      GUARD();
      children.push_back(std::move(op));
      children.push_back(std::move(inner));
    }
  } else {
    auto add_expr = NT_CONVERT(AddExpr);
    add_expr->match(ph);
    GUARD();
    children.push_back(std::move(add_expr));
  }
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

// CALL  -> IDENT CALL' | NUMBER | true | false | ( VALUE_EXPRESSION )
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

  } else if (CMP_CUR(TknType::TRUE)) {
    auto t = T_CONVERT(ph.curTkn.literal);
    expect(ph, TknType::TRUE);
    children.push_back(std::move(t));

  } else if (CMP_CUR(TknType::FALSE)) {
    auto f = T_CONVERT(ph.curTkn.literal);
    expect(ph, TknType::FALSE);
    children.push_back(std::move(f));

  } else if (CMP_CUR(TknType::LPAREN)) {
    expect(ph, TknType::LPAREN);  // consume (

    auto value_expr = NT_CONVERT(ValueExpression);
    value_expr->match(ph);
    GUARD();
    children.push_back(std::move(value_expr));

    expect(ph, TknType::RPAREN);
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
    auto value_expr = NT_CONVERT(ValueExpression);
    value_expr->match(ph);
    GUARD();
    children.push_back(std::move(value_expr));

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