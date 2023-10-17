# mlang

  

## Grammar notation Code representation in the parser

```
 Terminal     -> Code to match and consume a token
Nonterminal   -> Call to that rule's function
     |        -> if or switch statement
   * or +     -> while or for loop
     ?        -> if statement
```
  

## Operators

```
=     -> assignment
+=    -> add_equal
-=    -> sub_equal
*=    -> mul_equal
/=    -> div_equal
+     -> binary_add
-     -> binary_sub
*     -> binary_mul
/     -> binary_div
==    -> binary_equality
!=    -> binary_inequality
>     -> comparison_greater
<     -> comparison_less
>=    -> comparison_greater_equal
<=    -> comparison_less_equal
[]    -> arr_indexing
!     -> unary_not
-     -> unary_minus
()    -> boolean_operator (is_true member function -> override)
++x   -> prefix_increment
x++   -> postfix_increment
--x   -> prefix_decrement
x--   -> postfix_decrement
```

## Formal Grammar

```
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
post_op            -> postfix_increment | postfix_decrement | ( subscript | member_access | member_call )*
postfix_increment  -> primary "++"
postfix_decrement  -> primary "--"
subscript          -> primary "[" logic_or "]"
member_access      -> primary "." IDENTIFIER
member_call        -> primary "." IDENTIFIER "(" arguments? ")"
primary            -> NUMBER | STRING | "true" | "false" | "none" | "(" expression ")" | func_call | IDENTIFIER | ( "new" IDENTIFIER "(" arguments? ")" ) | ( "{" arguments "}" )
func_call          -> IDENTIFIER "(" arguments? ")"

arguments          -> logic_or ( "," logic_or )* 
block              -> "{" statement* "}"
```