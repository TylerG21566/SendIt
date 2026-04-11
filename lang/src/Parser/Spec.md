# Processing Let Statements

## let parsing reqs

variable bindings are statements of the following form:

  let x = 5;
  let y = 10;
  let foobar = add(5, 5);
  let barfoo = 5 * 5 / 10 + 18 - add(5, 5) + multiply(124);
  let anotherName = barfoo;

  let add = fn(a, b) {
  return a + b;
  };

These all take the form of:

  let <IDENT> = <EXPRESSION>;

## Working CFG: v1

// CORE STATEMENT SEQUENCE
PROGRAM          -> STATEMENT EOF
STATEMENT        -> LET_STATEMENT STATEMENT
                 -> RETURN_STATEMENT STATEMENT
                 -> E 
LET_STATEMENT    -> let IDENT = EXPRESSION ;
RETURN_STATEMENT -> return FORMULA;
EXPRESSION       -> FORMULA | FUNC_DEF


// OPERANDS AND OPERATORS
FORMULA          -> ADD_EXPR
ADD_EXPR         -> MUL_EXPR ADD_EXPR'
ADD_EXPR'        -> (+|-) MUL_EXPR ADD_EXPR' | E
MUL_EXPR         -> CALL MUL_EXPR'
MUL_EXPR'        -> (*|/) CALL MUL_EXPR' | E
CALL             -> IDENT CALL' | NUMBER | ( FORMULA )
CALL'            -> ( ARG ) | E


// FUNCTION INPUTS
PARAMS_DEF       -> E
                 -> IDENT PARAMS_DEF'
PARAMS_DEF'      -> , IDENT PARAMS_DEF' | E
ARG              -> E
                 -> FORMULA ARG'
ARG'             -> , FORMULA ARG' | E
 
// FUNCTION DEFINITION
MULTI_STATEMENT  -> STATEMENT MULTI_STATEMENT
                 -> E
FUNC_DEF         -> fn(PARAMS_DEEF){
  MULTI_STATEMENT
}

## Errors
EXPECTED_TKN_ERR: These happen when the parser expects a specific token but gets something else. Be specific about what you expected and what you got

UNEXPECTED_TKN_ERR: When the parser encounters something that doesn't match any rule

UNCLOSED_DELIMMITER_ERR: "Expected ')' to close '(' opened at line 1, col 9"

EOF_ERR: When input ends unexpectedly

## Working CFG: v2

// CORE STATEMENT SEQUENCE
PROGRAM          -> STATEMENT EOF
STATEMENT        -> LET_STATEMENT STATEMENT
                 -> RETURN_STATEMENT STATEMENT
                 -> IF_THEN_ELSE STATEMENT
                 -> E 
LET_STATEMENT    -> let IDENT = EXPRESSION ;
RETURN_STATEMENT -> return VALUE_EXPRESSION;
EXPRESSION       -> VALUE_EXPRESSION | FUNC_DEF

// if then else structure
IF               -> if ( VALUE_EXPRESSION ) { MULTI_STATEMENT } THEN_ELSE;
THEN_ELSE        -> else { MULTI_STATEMENT } | E

// Math and Boolean Precedence Chain
VALUE_EXPRESSION -> COMPARE
COMPARE          -> OR_EXPR COMPARE' 
COMPARE'         -> (==|!=|>|<) OR_EXPR | E
OR_EXPR          -> AND_EXPR OR_EXPR'
OR_EXPR'         -> or AND_EXPR OR_EXPR' | E
AND_EXPR         -> NOT_EXPR AND_EXPR'
AND_EXPR'        -> and NOT_EXPR AND_EXPR' | E
NOT_EXPR         -> not NOT_EXPR | ADD_EXPR
ADD_EXPR         -> MUL_EXPR ADD_EXPR'
ADD_EXPR'        -> (+|-) MUL_EXPR ADD_EXPR' | E
MUL_EXPR         -> CALL MUL_EXPR'
MUL_EXPR'        -> (*|/) CALL MUL_EXPR' | E
CALL             -> IDENT CALL' | NUMBER | true | false | ( VALUE_EXPRESSION )
CALL'            -> ( ARG ) | E

// FUNCTION INPUTS
PARAMS_DEF       -> E
                 -> IDENT PARAMS_DEF'
PARAMS_DEF'      -> , IDENT PARAMS_DEF' | E
ARG              -> E
                 -> VALUE_EXPRESSION ARG'
ARG'             -> , VALUE_EXPRESSION ARG' | E
 
// FUNCTION DEFINITION
MULTI_STATEMENT  -> STATEMENT MULTI_STATEMENT
                 -> E
FUNC_DEF         -> fn(PARAMS_DEEF){
  MULTI_STATEMENT
}

