# mlang

Embeddable interpreter for C++ projects. The library contains only the language core supporting the scripting language syntax with built-in basic data types (None, Boolean, String, Number, Array). Complex data types can be implemented on the C++ side of the project and instantiated and used in the script. Functions can be defined in both the script and the C++ side, allowing the user to hide proprietary implementations while still allowing for robust reconfiguration of the application without the need for a new build. Furthermore, the library does not provide support for IO operations and other complex algorithms that could issue concerns regarding security considerations. These functionalities can be implemented in the C++ host project allowing for compliance with internal cyber security policies. The library offers a flexible and robust extension to any C++ project that wants to offer a scripting interface but does not want to bind extensive scripting language interpreters such as Python or Lua. The language offers a familiar C-style syntax while supporting dynamic types.

```
function pow (a, b) {
    if (b <= 0) {
        return 0;
    }
    var result = a;
    for (; b > 1; --b) {
        result *= a;
    }
    return result;
}

var base = 2;
var exponent = 10;

var result = pow(base, exponent);

print("the result or %d^%d is %d\n", base, exponent, result);
```

output : the result of 2^10 is 1024
  

## Operators

The language supports the following built-in operators:

```
new   -> construct
=     -> assign
+=    -> operator_add_equal
-=    -> operator_sub_equal
*=    -> operator_mul_equal
/=    -> operator_div_equal
+     -> operator_binary_add
-     -> operator_binary_sub
*     -> operator_binary_mul
/     -> operator_binary_div
==    -> operator_comparison_equal
!=    -> operator_comparison_not_equal
>     -> operator_greater
<     -> operator_less
>=    -> operator_greater_equal
<=    -> operator_less_equal
[]    -> operator_subscript
!     -> unary_not
-     -> unary_minus
()    -> boolean_operator (is_true member function)
++x   -> prefix_increment
x++   -> postfix_increment
--x   -> prefix_decrement
x--   -> postfix_decrement
&&    -> operator_binary_and
||    -> operator_binary_or
```

## Formal Grammar

The formal grammar of the language, implemented by the parser, is the following:

```
program            -> declaration* EOF

declaration        -> func_decl | var_decl | statement
func_decl          -> "function" IDENTIFIER "(" ( IDENTIFIER ("," IDENTIFIER)* )? ")" block
var_decl           -> "var" IDENTIFIER ( "=" expression )? ";"

statement          -> exp_statement | if_statement | for_statement | while_statement | print_statement | control
exp_statement      -> expression ";"
if_statement       -> "if" "(" expression ")" block ( "else if" "(" expression ")" block )* ( "else" block )?
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

### Grammar notation Code representation in the parser

```
 Terminal     -> Code to match and consume a token
Nonterminal   -> Call to that rule's function
     |        -> if or switch statement
   * or +     -> while or for loop
     ?        -> if statement
```

## TODO

- namespace
- class
- switch-case
- typename (returns string containing the type name of the expression passed as argument)
- clarify function scopes (which variables, functions and types can they access)
- string : add functions like substring, ltrim, rtrim, cut, split, ...
- array : add some algorithms, like ordering, search, splitting, ...
- void return
- exit -> must get a Number
- return value of the whole script must be a Number -> 0 = success
- later : optimization step between parsing and executing (reduce performance cost of scripts executed periodically)
- logger -> to a configurable stream rather than to stdout