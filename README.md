# mlang

precedence going from lowest to highest:
equality   :  ==, !=
comparison :  > >= < <=
term       :  - +
factor     :  / *
unary      :  ! -


expression -> equality
equality   -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term       -> factor ( ( "-" | "+" ) factor )* ;
factor     -> unary ( ( "/" | "*" ) unary )* ;
unary      -> ( "-" | "!" ) unary | primary;
primary    -> NUMBER | STRING | "true" | "false" | "(" expression ")" | IDENTIFIER;


Grammar notation          Code representation in the parser
Terminal                  Code to match and consume a token
Nonterminal               Call to that rule's function
   |                      if or switch statement
 * or +                   while or for loop
   ?                      if statement



statement   -> declaration | expression | assignment | if | endif ";"
if          -> "if" "(" expression ")"
endif       -> "endif"
declaration -> "number" | "string" | "array" | "bool" IDENTIFIER ( "=" expression )?
assignment  -> IDENTIFIER ( ( "=" expression )* | ("+=" | "-=" | "*=" | "/=" expression) )+;
expression  -> equality
equality    -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison  -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term        -> factor ( ( "-" | "+" ) factor )* ;
factor      -> unary ( ( "/" | "*" ) unary )* ;
unary       -> ( "-" | "!" ) unary | primary;
primary     -> NUMBER | STRING | "true" | "false" | "(" expression ")" | IDENTIFIER;

TODO : ++, --, while, return, break, continue, functions, refine error handling

statement   -> declaration | expression | assignment | if | for | while | end | elif | else | print ";"
print       -> "print" "(" STRING ( "," expression )* ")"
if          -> "if" "(" expression ")"
elif        -> "elif" "(" expression ")"
else        -> "else"
while       -> "while" "(" expression ")"
for         -> "for" "(" assignment ( "," assignment )* ";" expression ( "," expression )* ";" expression ( "," expression )* ")"
end         -> "end"
declaration -> "number" | "string" | "array" | "bool" IDENTIFIER ( "=" expression )?
assignment  -> IDENTIFIER ( ( "=" expression )* | ("+=" | "-=" | "*=" | "/=" expression) )+;
expression  -> equality
equality    -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison  -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term        -> factor ( ( "-" | "+" ) factor )* ;
factor      -> unary ( ( "/" | "*" ) unary )* ;
unary       -> ( "-" | "!" ) unary | primary;
primary     -> NUMBER | STRING | "true" | "false" | "(" expression ")" | IDENTIFIER | "[" expression "]";