
# Grammar type suitablity

LR(left recursive) languages are suitable for botttom up parsing but not top down

LR
E -> E + T | T
T -> T * F | F
F -> ( E ) | id

non-LR version
E -> TE'
E' -> +TE'| e
T -> FT'
T' -> * F T' | e
F -> ( E ) | id


Ambigous CFG
E -> E+E | E*E | (E) | id

# Why not REGEX?

1. Lack of operation clarity, regex cannot infer that * occurs before +
2. Okay, lets just use parentheseses! Well, regex cannot ensure that parenthese open and closing is balanced

# Adding Precedence
To embed * priority over +, if we use post-order tree walk, we can just add extra layers for lower
precedent layers as below

 Goal → Expr
 Expr → Expr + Term
 | Expr - Term
 | Term
 Term → Term x Factor
 | Term ÷ Factor
 | Factor
 Factor → ( Expr )
 | num
 | name

here + has only 1 layer, * has 2, () has 3. This means that ()->*->+ is the order of operations

# CFG classes and their parsers
each of these are entirely contained by the more siginifcant class (so CFG contians everything for example)
CFG > LR(1) > LL(1) > RG

# What is Parsing?
Constructiing a derivation from a specific input sentence

# Tree Construction

Top-down parsers begin with the root and grow the tree toward the
leaves. At each step, a top-down parser selects a node for some
nonterminal on the lower fringe of the tree and extends it with a subtree
that represents the right-hand side of a production that rewrites the
nonterminal.

Bottom-up parsers begin with the leaves and grow the tree toward the
root. At each step, a bottom-up parser identifies a contiguous substring
of the parse tree’s upper fringe that matches the right-hand side of some
production; it then builds a node for the rule’s left-hand side and
connects it into the tree.

# Top down Parsing

root ← node for the start symbol, S ;
focus ← root;
push(null);
word ← NextWord( );
while (true) do;
  if (focus is a nonterminal) then begin;
    pick next rule to expand focus (A → β1, β2, . . . , βn);
    build nodes for β1, β2 . . . βn as children of focus;
    push(βn, βn−1, . . . , β2); # this is the order to push by the wat
    focus ← β1;
  end;
  else if (word matches focus) then begin;
    word ← NextWord( );
    focus ← pop( )
  end;
  else if (word = eof and focus = null)
    then accept the input and return root;
    else backtrack;
end;

A top-down parser begins with the root of the parse tree and systemati-
cally extends the tree downward until its leaves match the classified words
returned by the scanner. At each point, the process considers a partially built
parse tree. It selects a nonterminal symbol on the lower fringe of the tree
and extends it by adding children that correspond to the right-hand side of
some production for that nonterminal. It cannot extend the frontier from a
terminal. This process continues until either
  a. the fringe of the parse tree contains only terminal symbols, and the input
  stream has been exhausted, or
  b. a clear mismatch occurs between the fringe of the partially built parse
  tree and the input stream


# Left Recursion

With left-recursion, a top-down parser can loop indefinitely
without generating a leading terminal symbol that the parser can match (and
advance the input). Fortunately, we can reformulate a left-recursive grammar
so that it uses right recursion—any recursion involves the rightmost symbol
in a rule.

indirect left recursion, which occurs when a chain of rules such as
α→β, β→γ , and γ →αδ creates the situation that α →+ αδ

# Left Recursion Removal

impose an order on the nonterminals,A1, A2, . . . , An

for i ← 1 to n do;

    for j ← 1 to i - 1 do;
    if ∃ a production Ai→Aj γ
    then replace Ai→Aj γ with one or more
    productions that expand Aj
  
  rewrite the productions to eliminate
  any direct left recursion on A

# What will our CFG need to be in order to do our Top down Parser??

Recursive Decent parsers must be: 
- Deterministic
- Not Left Recurisive 
- NOT AMBIGOUS