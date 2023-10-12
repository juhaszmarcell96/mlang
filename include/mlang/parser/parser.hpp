#pragma once

#include <vector>

#include "mlang/script/token.hpp"
#include "mlang/ast/node.hpp"

#define TRACE_PARSER 0

#if TRACE_PARSER == 1
#include <iostream>
#endif

namespace mlang {
namespace parser {

class Parser {
private:
    std::vector<const script::Token*> m_tokens;

    std::size_t m_index { 0 };

    void next(int num = 1);
    bool done() const;
    const script::Token* peek (int num = 1) const;
    bool peekable (int num = 1);
    const script::Token* curr() const;
    const script::Token* prev() const;

    void trace (const std::string& str) const;

    bool consume (script::token_types type);
    void consume (script::token_types type, const std::string& err_msg);

    // primary            -> NUMBER | STRING | "true" | "false" | "none" | "(" expression ")" | IDENTIFIER | ( "new" IDENTIFIER "(" arguments? ")" ) | ( "{" arguments "}" )
    ast::node_ptr primary ();

    // post_op            -> postfix_increment | postfix_decrement | ( func_call | subscript | member_access | member_call )*
    // postfix_increment  -> primary "++"
    // postfix_decrement  -> primary "--"
    // func_call          -> primary "(" arguments? ")"
    // subscript          -> primary "[" logic_or "]"
    // member_access      -> primary "." IDENTIFIER
    // member_call        -> primary "." IDENTIFIER "(" arguments? ")"
    ast::node_ptr post_op ();

    // pre_op             -> ( "-" | "!" | "++" | "--" )? post_op;
    ast::node_ptr pre_op ();

    // factor             -> pre_op ( ( "/" | "*" ) pre_op )*
    ast::node_ptr factor ();

    // term               -> factor ( ( "-" | "+" ) factor )*
    ast::node_ptr term ();

    // comparison         -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
    ast::node_ptr comparison ();

    // equality           -> comparison ( ( "!=" | "==" ) comparison )*
    ast::node_ptr equality ();

    // logic_and          -> equality ( "&&" equality )*
    ast::node_ptr logic_and ();

    // logic_or           -> logic_and ( "||" logic_and )*
    ast::node_ptr logic_or ();

    // assignment         -> logic_or ( "=" | "+=" | "-=" | "*=" | "/=" logic_or )?
    ast::node_ptr assignment ();

    // expression         -> assignment
    ast::node_ptr expression ();

    // break_statement    -> "break" ";"
    ast::node_ptr break_statement ();

    // continue_statement -> "continue" ";"
    ast::node_ptr continue_statement ();

    // return_statement   -> "return" expression? ";"
    ast::node_ptr return_statement ();

    // exit_statement     -> "exit" expression ";"
    ast::node_ptr exit_statement ();

    // print_statement  -> "print" "(" STRING ( "," expression )* ")" ";"
    ast::node_ptr print_statement ();

    // while_statement  -> "while" "(" expression ")" block
    ast::node_ptr while_statement ();

    // for_statement    -> "for" "(" ( var_decl | exp_statement | ";" ) expression? ";" expression? ")" block
    ast::node_ptr for_statement ();

    // if_statement     -> "if" "(" expression ")" block ( "elif" "(" expression ")" block )* ( "else" block )?
    ast::node_ptr if_statement ();

    // exp_statement    -> expression ";"
    ast::node_ptr exp_statement ();

    // statement        -> exp_statement | if_statement | for_statement | while_statement | print_statement | control
    ast::node_ptr statement ();

    // block            -> "{" statement* "}"
    ast::node_ptr block ();

    // var_decl         -> "var" IDENTIFIER ( "=" expression )? ";"
    ast::node_ptr var_decl ();

    // func_decl        -> "function" IDENTIFIER "(" ( IDENTIFIER ("," IDENTIFIER)* )? ")" block
    ast::node_ptr func_decl ();

    // declaration      -> func_decl | var_decl | statement
    ast::node_ptr declaration ();

public:
    Parser () = default;
    ~Parser () = default;

    ast::node_ptr parse (const std::vector<script::Token>& tokens);
};

} /* namespace parser */
} /* namespace mlang */