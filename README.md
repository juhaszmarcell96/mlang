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

TODO : ++, --, return, break, continue, functions, refine error handling, switch-case, array handling, pointer/reference type

statement   -> declaration | expression | assignment | if | for | while | end | elif | else | print | control ";"
print       -> "print" "(" STRING ( "," expression )* ")"
if          -> "if" "(" expression ")"
elif        -> "elif" "(" expression ")"
else        -> "else"
while       -> "while" "(" expression ")"
control     -> "break" | "continue" | ("return" expression)
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


primary     -> NUMBER | STRING | "true" | "false" | "(" expression ")" | lvalue;
lvalue      -> ( "++" | "--" )? IDENTIFIER ("[" expression "]")?

TODO : shadowing -> can redefine a variable in the local scope



operators:
    =    -> assignment
    +=   -> add_equal
    -=   -> sub_equal
    *=   -> mul_equal
    /=   -> div_equal
    +    -> binary_add
    -    -> binary_sub
    *    -> binary_mul
    /    -> binary_div
    ==   -> binary_equality
    !=   -> binary_inequality
    >    -> comparison_greater
    <    -> comparison_less
    >=   -> comparison_greater_equal
    <=   -> comparison_less_equal
    []   -> arr_indexing
    !    -> unary_not
    -    -> unary_minus
    ()   -> boolean_operator (is_true member function -> override)
    
    ++x  -> prefix_increment
    x++  -> postfix_increment
    --x  -> prefix_decrement
    x--  -> postfix_decrement








statement   -> declaration | expression | assignment | if | for | while | end | elif | else | print | control | func_decl ";"
func_decl   -> "function" IDENTIFIER "(" ( typename IDENTIFIER ("," typename IDENTIFIER)* )? ")" "-" ">" typename;
print       -> "print" "(" STRING ( "," expression )* ")"
if          -> "if" "(" expression ")"
elif        -> "elif" "(" expression ")"
else        -> "else"
while       -> "while" "(" expression ")"
control     -> "break" | "continue" | ("return" expression) | ("exit" expression )
for         -> "for" "(" assignment ( "," assignment )* ";" expression ( "," expression )* ";" expression ( "," expression )* ")"
end         -> "end"
declaration -> "number" | "string" | "array" | "bool" IDENTIFIER ( "=" expression )?
assignment  -> lvalue ( ( "=" assignment )* | ("+=" | "-=" | "*=" | "/=" expression) )+;
expression  -> equality
equality    -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison  -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term        -> factor ( ( "-" | "+" ) factor )* ;
factor      -> unary ( ( "/" | "*" ) unary )* ;
unary       -> ( "-" | "!" ) unary | primary;
primary     -> NUMBER | STRING | "true" | "false" | "(" expression ")" | lvalue | func_call | member_call;
lvalue      -> ( "++" | "--" )? IDENTIFIER ("[" expression "]")?
member_call -> IDENTIFIER ("[" expression "]")? "." IDENTIFIER "(" ( expression ("," expression)* )? ")"
func_call   -> IDENTIFIER "(" (expression)* ")"

typename    -> "number" | "string" | "array" | "bool"









program            -> declaration* EOF

declaration        -> func_decl | var_decl | statement
func_decl          -> "function" IDENTIFIER "(" ( IDENTIFIER ("," IDENTIFIER)* )? ")" block
var_decl           -> "var" IDENTIFIER ( "=" expression )? ";"

statement          -> exp_statement | if_statement | for_statement | while_statement | print_statement | control
exp_statement      -> expression ";"
if_statement       -> "if" "(" expression ")" block ( "elif" "(" expression ")" block )* ( "else" block )?
for_statement      -> "for" "(" ( var_decl | exp_statement | ";" ) expression? ";" expression? ")" block
while_statement    -> "while" "(" expression ")" block
print_statement    -> "print" "(" STRING ( "," expression )* ")" ";"
control            -> break_statement | continue_statement | return_statement | exit_statement
break_statement    -> "break" ";"
continue_statement -> "continue" ";"
return_statement   -> "return" expression? ";"
exit_statement     -> "exit" expression ";"

expression         -> assignment
assignment         -> logic_or ( "=" | "+=" | "-=" | "*=" | "/=" logic_or )?
logic_or           -> logic_and ( "||" logic_and )*
logic_and          -> equality ( "&&" equality )*
equality           -> comparison ( ( "!=" | "==" ) comparison )*
comparison         -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
term               -> factor ( ( "-" | "+" ) factor )*
factor             -> pre_op ( ( "/" | "*" ) pre_op )*
pre_op             -> ( "-" | "!" | "++" | "--" )? post_op;
post_op            -> postfix_increment | postfix_decrement | ( func_call | subscript | member_access | member_call )*
postfix_increment  -> primary "++"
postfix_decrement  -> primary "--"
func_call          -> primary "(" arguments? ")"
subscript          -> primary "[" logic_or "]"
member_access      -> primary "." IDENTIFIER
member_call        -> primary "." IDENTIFIER "(" arguments? ")"
primary            -> NUMBER | STRING | "true" | "false" | "none" | "(" expression ")" | IDENTIFIER | ( "new" IDENTIFIER "(" arguments? ")" ) | ( "{" arguments "}" )

arguments          -> logic_or ( "," logic_or )* 
block              -> "{" statement* "}"








program            -> declaration* EOF

declaration        -> func_decl | var_decl | statement
func_decl          -> "function" IDENTIFIER "(" ( IDENTIFIER ("," IDENTIFIER)* )? ")" block
var_decl           -> "var" IDENTIFIER ( "=" expression )? ";"

statement          -> exp_statement | if_statement | for_statement | while_statement | print_statement | control
exp_statement      -> expression ";"
if_statement       -> "if" "(" expression ")" block ( "elif" "(" expression ")" block )* ( "else" block )?
for_statement      -> "for" "(" ( var_decl | exp_statement | ";" ) expression? ";" expression? ")" block
while_statement    -> "while" "(" expression ")" block
print_statement    -> "print" "(" STRING ( "," expression )* ")" ";"
control            -> break_statement | continue_statement | return_statement | exit_statement
break_statement    -> "break" ";"
continue_statement -> "continue" ";"
return_statement   -> "return" expression? ";"
exit_statement     -> "exit" expression ";"

expression         -> assignment
assignment         -> ( lvalue ( "=" | "+=" | "-=" | "*=" | "/=" ) )? logic_or
logic_or           -> logic_and ( "||" logic_and )*
logic_and          -> equality ( "&&" equality )*
equality           -> comparison ( ( "!=" | "==" ) comparison )*
comparison         -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
term               -> factor ( ( "-" | "+" ) factor )*
factor             -> pre_op ( ( "/" | "*" ) pre_op )*
pre_op             -> ( "-" | "!" | "++" | "--" )? post_op;
post_op            -> primary ( "++" | "--" | ( "(" arguments? ")" ) | ( ("[" expression "]") | ("." IDENTIFIER ( "(" arguments? ")" )) )* )?
primary            -> NUMBER | STRING | "true" | "false" | "none" | "(" expression ")" | IDENTIFIER | ( "new" IDENTIFIER "(" arguments? ")" ) | ( "{" arguments "}" )
lvalue             -> IDENTIFIER ( subscript | member_access )*
subscript          -> "[" expression "]"
member_access      -> "." IDENTIFIER

arguments          -> logic_or ( "," logic_or )* 
block              -> "{" statement* "}"