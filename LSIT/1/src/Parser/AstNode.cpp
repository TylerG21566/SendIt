#include "Parser/AstNode.h"

#include <memory>
#include <string>
#include <vector>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"

/***
Parser Head
***/

ParserHead::ParserHead(std::vector<TokenStruct> TokenVector)
    : tokens(std::make_move_iterator(TokenVector.begin()),
             std::make_move_iterator(TokenVector.end())) {
  nextToken();  // prime lookahead on construction
  nextToken();  // prime curTkn too, if you want both ready
}

void ParserHead::nextToken() {
  curTkn = std::move(lookahead);

  if (tokens.empty()) {
    lookahead = TokenStruct{TknType::END_F, ""};
  } else {
    lookahead = std::move(tokens.front());
    tokens.pop_front();
  }
}

/***
Non-Terminals
***/

Ast::Terminal::Terminal(std::string v) { this->value = std::move(v); }
std::string Ast::Terminal::toString() { return value; };
void Ast::Terminal::match(ParserHead& ph) {};

/* ****************************

CORE STATEMENT SEQUENCE

**************************** */

// Program
STR_SET(Program)
void Ast::Program::match(ParserHead& ph) {
    if (ph.hasError()) return;
    auto stat = NT_CONVERT(Statement);
    stat->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(stat));
    expect(ph, TknType::END_F);
}

// Statement
STR_SET(Statement)
void Ast::Statement::match(ParserHead& ph) {
    if (ph.hasError()) return;
    GET_TOKENS();

    if (CMP_CUR(TknType::LET)) {
        auto let = NT_CONVERT(LetStatement);
        let->match(ph);
        if (ph.hasError()) return;
        children.push_back(std::move(let));

        auto next = NT_CONVERT(Statement);
        next->match(ph);
        if (ph.hasError()) return;
        children.push_back(std::move(next));
    }
    else if (CMP_CUR(TknType::RETURN)) {
        auto ret = NT_CONVERT(ReturnStatement);
        ret->match(ph);
        if (ph.hasError()) return;
        children.push_back(std::move(ret));

        auto next = NT_CONVERT(Statement);
        next->match(ph);
        if (ph.hasError()) return;
        children.push_back(std::move(next));
    }
    // else epsilon — do nothing
}

STR_SET(LetStatement)
void Ast::LetStatement::match(ParserHead& ph) {
  if (ph.hasError()) return;

  expect(ph, TknType::LET);

  auto ident = T_CONVERT(ph.curTkn.literal);
  expect(ph, TknType::IDENT);
  expect(ph, TknType::ASSIGN);

  auto expr = NT_CONVERT(Expression);
  expr->match(ph);
  if (ph.hasError()) return;

  expect(ph, TknType::SEMICOLON);

  children.push_back(std::move(ident));
  children.push_back(std::move(expr));
}

STR_SET(ReturnStatement)
void Ast::ReturnStatement::match(ParserHead& ph) {
    if (ph.hasError()) return;
    expect(ph, TknType::RETURN);

    auto formula = NT_CONVERT(Formula);
    formula->match(ph);
    if (ph.hasError()) return;

    expect(ph, TknType::SEMICOLON);
    children.push_back(std::move(formula));
}

STR_SET(Expression)
void Ast::Expression::match(ParserHead& ph) {
  if (ph.hasError()) return;

  GET_TOKENS();

  if (CMP_CUR(TknType::FUNC)) {
    auto func_def = NT_CONVERT(FuncDef);
    func_def->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(func_def));
  } else if (CMP_CUR(TknType::IDENT) || CMP_CUR(TknType::INT) ||
             CMP_CUR(TknType::LPAREN)) {
    auto formula = NT_CONVERT(Formula);
    formula->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(formula));
  } else {
    ph.addError(cur, UNEXPECTED_TKN_ERR);
  };
};

STR_SET(Formula)
void Ast::Formula::match(ParserHead& ph) {
  if (ph.hasError()) return;  // stop on first failure
  // parsing production rule logic
  GET_TOKENS();
  if (CMP_CUR(TknType::IDENT) || CMP_CUR(TknType::INT) ||
      CMP_CUR(TknType::LPAREN)) {
    auto add_expr = NT_CONVERT(AddExpr);
    add_expr->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(add_expr));
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
    if (ph.hasError()) return;
    auto add_expr = NT_CONVERT(AddExpr);
    add_expr->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(add_expr));
}

// ADD_EXPR -> MUL_EXPR ADD_EXPR'
// ADD_EXPR' -> (+|-) MUL_EXPR ADD_EXPR' | E
STR_SET(AddExpr)
void Ast::AddExpr::match(ParserHead& ph) {
    if (ph.hasError()) return;

    auto mul = NT_CONVERT(MulExpr);
    mul->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(mul));

    // ADD_EXPR' as a loop
    while (ph.curTkn.type == TknType::PLUS ||
           ph.curTkn.type == TknType::MINUS) {
        auto op = T_CONVERT(ph.curTkn.literal);
        ph.nextToken();

        auto next_mul = NT_CONVERT(MulExpr);
        next_mul->match(ph);
        if (ph.hasError()) return;

        children.push_back(std::move(op));
        children.push_back(std::move(next_mul));
    }
}

// MUL_EXPR -> UNARY MUL_EXPR'
// MUL_EXPR' -> (*|/) UNARY MUL_EXPR' | E
STR_SET(MulExpr)
void Ast::MulExpr::match(ParserHead& ph) {
    if (ph.hasError()) return;

    auto call = NT_CONVERT(Call);
    call->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(call));

    // MUL_EXPR' as a loop
    while (ph.curTkn.type == TknType::ASTERISK ||
           ph.curTkn.type == TknType::SLASH) {
        auto op = T_CONVERT(ph.curTkn.literal);
        ph.nextToken();

        auto next_call = NT_CONVERT(Call);
        next_call->match(ph);
        if (ph.hasError()) return;

        children.push_back(std::move(op));
        children.push_back(std::move(next_call));
    }
}

// CALL -> IDENT CALL' | NUMBER | ( FORMULA )
// CALL' -> ( ARG ) | E
STR_SET(Call)
void Ast::Call::match(ParserHead& ph) {
    if (ph.hasError()) return;
    GET_TOKENS();

    if (CMP_CUR(TknType::IDENT)) {
        auto ident = T_CONVERT(ph.curTkn.literal);
        ph.nextToken();

        // CALL' -> ( ARG ) | E
        if (ph.curTkn.type == TknType::LPAREN) {
            ph.nextToken();  // consume (

            auto args = NT_CONVERT(Arg);
            args->match(ph);
            if (ph.hasError()) return;

            expect(ph, TknType::RPAREN);

            children.push_back(std::move(ident));
            children.push_back(std::move(args));
        } else {
            // just an identifier, no call
            children.push_back(std::move(ident));
        }
    }
    else if (CMP_CUR(TknType::INT)) {
        auto num = T_CONVERT(ph.curTkn.literal);
        ph.nextToken();
        children.push_back(std::move(num));
    }
    else if (CMP_CUR(TknType::LPAREN)) {
        ph.nextToken();  // consume (

        auto formula = NT_CONVERT(Formula);
        formula->match(ph);
        if (ph.hasError()) return;

        expect(ph, TknType::RPAREN);
        children.push_back(std::move(formula));
    }
    else {
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
    if (ph.hasError()) return;

    // E case — no arguments
    if (ph.curTkn.type == TknType::RPAREN) return;

    auto formula = NT_CONVERT(Formula);
    formula->match(ph);
    if (ph.hasError()) return;
    children.push_back(std::move(formula));

    // ARG' as a loop
    while (ph.curTkn.type == TknType::COMMA) {
        ph.nextToken();  // consume ,

        auto next_formula = NT_CONVERT(Formula);
        next_formula->match(ph);
        if (ph.hasError()) return;
        children.push_back(std::move(next_formula));
    }
}

// PARAMS_DEF -> E | IDENT PARAMS_DEF'
// PARAMS_DEF' -> , IDENT PARAMS_DEF' | E
STR_SET(ParamsDef)
void Ast::ParamsDef::match(ParserHead& ph) {
    if (ph.hasError()) return;

    // E case — no params
    if (ph.curTkn.type == TknType::RPAREN) return;

    if (ph.curTkn.type != TknType::IDENT) {
        ph.addError(ph.curTkn, EXPECTED_TKN_ERR);
        return;
    }
    auto ident = T_CONVERT(ph.curTkn.literal);
    ph.nextToken();
    children.push_back(std::move(ident));

    // PARAMS_DEF' as a loop
    while (ph.curTkn.type == TknType::COMMA) {
        ph.nextToken();  // consume ,

        if (ph.curTkn.type != TknType::IDENT) {
            ph.addError(ph.curTkn, EXPECTED_TKN_ERR);
            return;
        }
        auto next_ident = T_CONVERT(ph.curTkn.literal);
        ph.nextToken();
        children.push_back(std::move(next_ident));
    }
}

/* ****************************
   FUNCTION DEFINITION
**************************** */

// MULTI_STATEMENT -> STATEMENT MULTI_STATEMENT | E
STR_SET(MultiStatement)
void Ast::MultiStatement::match(ParserHead& ph) {
    if (ph.hasError()) return;

    
}

// FUNC_DEF -> fn ( PARAMS_DEF ) { MULTI_STATEMENT }
STR_SET(FuncDef)
void Ast::FuncDef::match(ParserHead& ph) {
    if (ph.hasError()) return;

    expect(ph, TknType::FUNC);      // consume fn
    expect(ph, TknType::LPAREN);    // consume (

    auto params = NT_CONVERT(ParamsDef);
    params->match(ph);
    if (ph.hasError()) return;

    expect(ph, TknType::RPAREN);    // consume )
    expect(ph, TknType::LBRACE);    // consume {

    auto body = NT_CONVERT(MultiStatement);
    body->match(ph);
    if (ph.hasError()) return;

    expect(ph, TknType::RBRACE);    // consume }

    children.push_back(std::move(params));
    children.push_back(std::move(body));
}