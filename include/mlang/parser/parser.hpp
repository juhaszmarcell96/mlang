#pragma once

#include <vector>

#include "mlang/token.hpp"
#include "mlang/environment.hpp"
#include "mlang/exception.hpp"
#include "mlang/parser/ast.hpp"

#include "mlang/object/object.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/boolean.hpp"

#define TRACE_PARSER 0

#if TRACE_PARSER == 1
#include <iostream>
#endif



namespace mlang {

typedef std::vector<const Token*> tokens_vec;

class Parser {
private:
    tokens_vec m_tokens;
    Node* m_current_scope { nullptr };
    Node* m_next_scope { nullptr };

    std::size_t m_index { 0 };

    void next(int num = 1) { m_index = std::min(m_index + num, m_tokens.size()); }
    bool done() const { return m_index >= m_tokens.size(); }
    const Token* peek (int num = 1) const {
        if ((m_index + num >= 0) && (m_index + num < m_tokens.size())) {
            return m_tokens[m_index + num];
        }
        return nullptr;
    }
    bool peekable (int num = 1) {
        if ((m_index + num) < 0) return false;
        if ((m_index + num) >= m_tokens.size()) return false;
        return true;
    }
    const Token* curr() const { return m_tokens[m_index]; }
    const Token* prev() const {
        if (m_index == 0) return nullptr;
        return m_tokens[m_index - 1];
    }

    void trace (const std::string& str) const {
        #if TRACE_PARSER == 1
        std::cout << str << std::endl;
        #endif
    }

    bool consume (token_types type) {
        if (done()) { throw syntax_error{ "statement terminated unexpectedly", prev()->line, prev()->pos}; }
        if (curr()->type == type) {
            next();
            return true;
        }
        return false;
    }

    void consume (token_types type, const std::string& err_msg) {
        if (done()) { throw syntax_error{ err_msg, prev()->line, prev()->pos}; }
        if (curr()->type != type) {
            throw syntax_error{ err_msg, curr()->line, curr()->pos};
        }
        next();
    }

    // primary            -> NUMBER | STRING | "true" | "false" | "none" | "(" expression ")" | IDENTIFIER | ( "new" IDENTIFIER "(" arguments? ")" ) | ( "{" arguments "}" )
    node_ptr primary () {
        trace("primary");
        if (consume(token_types::number)) { return std::make_unique<ValueNode>(std::make_shared<Number>(prev()->value_num, false)); }
        if (consume(token_types::string)) { return std::make_unique<ValueNode>(std::make_shared<String>(prev()->value_str, false)); }
        if (consume(token_types::kw_true)) { return std::make_unique<ValueNode>(std::make_shared<Boolean>(true, false)); }
        if (consume(token_types::kw_false)) { return std::make_unique<ValueNode>(std::make_shared<Boolean>(false, false)); }
        if (consume(token_types::round_bracket_open)) {
            node_ptr expr = expression();
            consume(token_types::round_bracket_close, "missing ')'");
            return expr;
        }
        if (consume(token_types::identifier)) { return std::make_unique<VariableNode>(prev()->value_str); }
        //if (consume(token_types::kw_new)) { return std::make_unique<VariableNode>(prev()->value_str); }
        if (consume(token_types::curly_bracket_open)) {
            std::unique_ptr<ArrayNode> arr_ptr = std::make_unique<ArrayNode>();
            while (!consume(token_types::curly_bracket_close)) {
                arr_ptr->add_element(logic_or());
                if (consume(token_types::comma)) {
                    if (consume(token_types::curly_bracket_close)) {
                        throw syntax_error{ "',' is followed by '}', which is invalid", prev()->line, prev()->pos};
                    }
                }
            }
            return arr_ptr;
        }
        throw syntax_error{ "unexpected primary token", prev()->line, prev()->pos};
        return nullptr;
    }

    // post_op            -> postfix_increment | postfix_decrement | ( func_call | subscript | member_access | member_call )*
    // postfix_increment  -> primary "++"
    // postfix_decrement  -> primary "--"
    // func_call          -> primary "(" arguments? ")"
    // subscript          -> primary "[" logic_or "]"
    // member_access      -> primary "." IDENTIFIER
    // member_call        -> primary "." IDENTIFIER "(" arguments? ")"
    node_ptr post_op () {
        trace("post_op");
        node_ptr expr = primary();
        while (true) {
            if (consume(token_types::plus_plus)) {
                expr = std::make_unique<PostfixIncrementNode>(std::move(expr));
                break;
            }
            else if (consume(token_types::dash_dash)) {
                expr = std::make_unique<PostfixDecrementNode>(std::move(expr));
                break;
            }
            else if (consume(token_types::round_bracket_open)) {
                std::unique_ptr<FunctionCallNode> func_call_ptr = std::make_unique<FunctionCallNode>(std::move(expr));
                while (!consume(token_types::round_bracket_close)) {
                    func_call_ptr->add_argument(logic_or());
                }
                expr.swap(func_call_ptr);
            }
            else if (consume(token_types::square_bracket_open)) {
                std::unique_ptr<SubscriptNode> subscript_ptr = std::make_unique<SubscriptNode>(std::move(expr));
                subscript_ptr->set_index(logic_or());
                consume(token_types::square_bracket_close, "missing ']' as subscript termination");
                expr.swap(subscript_ptr);
            }
            else if (consume(token_types::dot)) {
                consume(token_types::identifier, "missing identifier in member access");
                std::string member_name = prev()->value_str;
                if (consume(token_types::round_bracket_open)) {
                    std::unique_ptr<MemberFunctionNode> member_func_ptr = std::make_unique<MemberFunctionNode>(std::move(expr), member_name);
                    while (!consume(token_types::round_bracket_close)) {
                        member_func_ptr->add_argument(logic_or());
                    }
                    expr.swap(member_func_ptr);
                }
                else {
                    std::unique_ptr<MemberAccessNode> member_access_ptr = std::make_unique<MemberAccessNode>(std::move(expr), member_name);
                    expr.swap(member_access_ptr);
                }
            }
            else {
                break;
            }
        }
        return expr;
    }

    // pre_op             -> ( "-" | "!" | "++" | "--" )? post_op;
    node_ptr pre_op () {
        trace("pre_op");
        while (true) {
            if (consume(token_types::dash)) {
                node_ptr rhs = post_op();
                return expr = std::make_unique<BinaryMulOperationNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::exclamation_mark)) {
                node_ptr rhs = post_op();
                return std::make_unique<BinaryDivOperationNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::plus_plus)) {
                node_ptr rhs = post_op();
                return std::make_unique<PrefixIncrementNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::dash_dash)) {
                node_ptr rhs = post_op();
                return std::make_unique<PrefixDecrementNode>(std::move(expr), std::move(rhs));
            }
            else {
                break;
            }
        }
        return post_op();
    }

    // factor             -> pre_op ( ( "/" | "*" ) pre_op )*
    node_ptr factor () {
        trace("factor");
        node_ptr expr = pre_op();
        while (true) {
            if (consume(token_types::asterisk)) {
                node_ptr rhs = pre_op();
                expr = std::make_unique<BinaryMulOperationNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::slash)) {
                node_ptr rhs = pre_op();
                expr = std::make_unique<BinaryDivOperationNode>(std::move(expr), std::move(rhs));
            }
            else {
                break;
            }
        }
        return expr;
    }

    // term               -> factor ( ( "-" | "+" ) factor )*
    node_ptr term () {
        trace("term");
        node_ptr expr = factor();
        while (true) {
            if (consume(token_types::plus)) {
                node_ptr rhs = comparison();
                expr = std::make_unique<BinaryAddOperationNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::dash)) {
                node_ptr rhs = comparison();
                expr = std::make_unique<BinarySubOperationNode>(std::move(expr), std::move(rhs));
            }
            else {
                break;
            }
        }
        return expr;
    }

    // comparison         -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
    node_ptr comparison () {
        trace("comparison");
        node_ptr expr = term();
        while (true) {
            if (consume(token_types::greater)) {
                node_ptr rhs = term();
                expr = std::make_unique<ComparisonGreaterNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::less)) {
                node_ptr rhs = term();
                expr = std::make_unique<ComparisonLessNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::greater_equal)) {
                node_ptr rhs = term();
                expr = std::make_unique<ComparisonGreaterEqualNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::less_equal)) {
                node_ptr rhs = term();
                expr = std::make_unique<ComparisonLessEqualNode>(std::move(expr), std::move(rhs));
            }
            else {
                break;
            }
        }
        return expr;
    }

    // equality           -> comparison ( ( "!=" | "==" ) comparison )*
    node_ptr equality () {
        trace("equality");
        node_ptr expr = comparison();
        while (true) {
            if (consume(token_types::exclamation_equal)) {
                node_ptr rhs = comparison();
                expr = std::make_unique<BinaryInequalityOperationNode>(std::move(expr), std::move(rhs));
            }
            else if (consume(token_types::double_equal)) {
                node_ptr rhs = comparison();
                expr = std::make_unique<BinaryEqualityOperationNode>(std::move(expr), std::move(rhs));
            }
            else {
                break;
            }
        }
        return expr;
    }

    // logic_and          -> equality ( "&&" equality )*
    node_ptr logic_and () {
        trace("logic_and");
        node_ptr expr = equality();
        while (consume(token_types::double_ampersand)) {
            node_ptr rhs = equality();
            expr = std::make_unique<BinaryAndOperationNode>(std::move(expr), std::move(rhs));
        }
        return expr;
    }

    // logic_or           -> logic_and ( "||" logic_and )*
    node_ptr logic_or () {
        trace("logic_or");
        node_ptr expr = logic_and();
        while (consume(token_types::double_pipe)) {
            node_ptr rhs = logic_and();
            expr = std::make_unique<BinaryOrOperationNode>(std::move(expr), std::move(rhs));
        }
        return expr;
    }

    // assignment         -> logic_or ( "=" | "+=" | "-=" | "*=" | "/=" logic_or )?
    node_ptr assignment () {
        trace("assignment");
        node_ptr expr = logic_or();
        if (consume(token_types::equal_sign)) {
            node_ptr rhs = logic_or();
            return std::make_unique<AssignmentOperationNode>(std::move(expr), std::move(rhs));
        }
        if (consume(token_types::plus_equal)) {
            node_ptr rhs = logic_or();
            return std::make_unique<AddEqualOperationNode>(std::move(expr), std::move(rhs));
        }
        if (consume(token_types::dash_equal)) {
            node_ptr rhs = logic_or();
            return std::make_unique<SubEqualOperationNode>(std::move(expr), std::move(rhs));
        }
        if (consume(token_types::asterisk_equal)) {
            node_ptr rhs = logic_or();
            return std::make_unique<MulEqualOperationNode>(std::move(expr), std::move(rhs));
        }
        if (consume(token_types::slash_equal)) {
            node_ptr rhs = logic_or();
            return std::make_unique<DivEqualOperationNode>(std::move(expr), std::move(rhs));
        }
        return expr;
    }

    // expression         -> assignment
    node_ptr expression () {
        trace("expression");
        return assignment();
    }

    // break_statement    -> "break" ";"
    node_ptr break_statement () {
        trace("break_statement");
        consume(token_types::semicolon, "missing ';' as 'break' statement termination");
        return std::make_unique<BreakNode>();
    }

    // continue_statement -> "continue" ";"
    node_ptr continue_statement () {
        trace("continue_statement");
        consume(token_types::semicolon, "missing ';' as 'continue' statement termination");
        return std::make_unique<ContinueNode>();
    }

    // return_statement   -> "return" expression? ";"
    node_ptr return_statement () {
        trace("return_statement");
        node_ptr value = expression();
        /* TODO : optional return value */
        consume(token_types::semicolon, "missing ';' as 'return' statement termination");
        return std::make_unique<ReturnNode>(std::move(value));
    }

    // exit_statement     -> "exit" expression ";"
    node_ptr exit_statement () {
        trace("exit_statement");
        node_ptr value = expression();
        consume(token_types::semicolon, "missing ';' as 'exit' statement termination");
        return std::make_unique<ExitNode>(std::move(value));
    }

    // print_statement  -> "print" "(" STRING ( "," expression )* ")" ";"
    node_ptr print_statement () {
        trace("print_statement");
        std::unique_ptr<PrintNode> print_node_ptr = std::make_unique<PrintNode>();
        consume(token_types::round_bracket_open, "missing '(' after 'print'");
        consume(token_types::string, "first parameter of 'print' must be a string");
        print_node_ptr->set_rule(prev()->value_str);
        while (!consume(token_types::round_bracket_close)) {
            consume(token_types::comma, "missing ',' delimiter in 'print' statement");
            print_node_ptr->add_argument(expression());
        }
        consume(token_types::semicolon, "missing ';' as 'print' statement termination");
        return print_node_ptr;
    }

    // while_statement  -> "while" "(" expression ")" block
    node_ptr while_statement () {
        trace("while_statement");
        std::unique_ptr<WhileStatementNode> while_node_ptr = std::make_unique<WhileStatementNode>();
        consume(token_types::round_bracket_open, "missing '(' after 'while'");
        while_node_ptr->set_condition(expression());
        consume(token_types::round_bracket_close, "missing ')' after 'while' statement condition");
        while_node_ptr->set_body(block());
        return while_node_ptr;
    }

    // for_statement    -> "for" "(" ( var_decl | exp_statement | ";" ) expression? ";" expression? ")" block
    node_ptr for_statement () {
        trace("for_statement");
        std::unique_ptr<ForStatementNode> for_node_ptr = std::make_unique<ForStatementNode>();
        consume(token_types::round_bracket_open, "missing '(' after 'for'");
        /* processing initialization (no multiple initializations supported) */
        if (consume(token_types::kw_var)) { for_node_ptr->set_initialization(var_decl()); }
        else if (consume(token_types::semicolon)) { /* nothing to do, no initialization found */ }
        else { for_node_ptr->set_initialization(exp_statement()); }
        /* processing tests (again, && and || are OK, but ',' is not) */
        if (consume(token_types::semicolon)) { /* nothing to do, no test found */ }
        else { for_node_ptr->set_test(expression()); }
        consume(token_types::semicolon, "missing ';' after 'for' loop test");
        /* processing update (again, no multiple updates supported) */
        if (consume(token_types::semicolon)) { /* nothing to do, no update found */ }
        else { for_node_ptr->set_update(expression()); }
        consume(token_types::round_bracket_close, "missing ')' after 'for' loop specification");
        for_node_ptr->set_body(block());
        return for_node_ptr();
    }

    // if_statement     -> "if" "(" expression ")" block ( "elif" "(" expression ")" block )* ( "else" block )?
    node_ptr if_statement () {
        trace("if_statement");
        std::unique_ptr<IfStatementNode> if_node_ptr = std::make_unique<IfStatementNode>();
        consume(token_types::round_bracket_open, "missing '(' after 'if'");
        if_node_ptr->set_if_condition(expression());
        consume(token_types::round_bracket_close, "missing ')' after 'if' statement condition");
        if_node_ptr->add_block(block());
        while (consume(token_types::kw_elif)) {
            consume(token_types::round_bracket_open, "missing '(' after 'elif'");
            if_node_ptr->add_elif_condition(expression());
            consume(token_types::round_bracket_close, "missing ')' after 'elif' statement condition");
            if_node_ptr->add_block(block());
        }
        if (consume(token_types::kw_else)) {
            if_node_ptr->add_else();
            if_node_ptr->add_block(block());
        }
        return if_node_ptr;
    }

    // exp_statement    -> expression ";"
    node_ptr exp_statement () {
        trace("exp_statement");
        node_ptr exp = expression();
        consume(token_types::semicolon, "missing ';' at expression termination");
        return exp;
    }

    // statement        -> exp_statement | if_statement | for_statement | while_statement | print_statement | control
    node_ptr statement () {
        trace("statement");
        if (consume(token_types::kw_if)) { return if_statement(); }
        if (consume(token_types::kw_for)) { return for_statement(); }
        if (consume(token_types::kw_while)) { return while_statement(); }
        if (consume(token_types::kw_print)) { return print_statement(); }
        if (consume(token_types::kw_break)) { return break_statement(); }
        if (consume(token_types::kw_continue)) { return continue_statement(); }
        if (consume(token_types::kw_return)) { return return_statement(); }
        if (consume(token_types::kw_exit)) { return exit_statement(); }
        return exp_statement();
    }

    // block            -> "{" statement* "}"
    node_ptr block () {
        trace("block");
        consume(token_types::curly_bracket_open, "missing scope opening '{' token");
        std::unique_ptr<BlockNode> block_ptr = std::make_unique<BlockNode>();
        while (!consume(token_types::curly_bracket_close)) {
            block_ptr->add_node(statement());
        }
        return block_ptr;
    }

    // var_decl         -> "var" IDENTIFIER ( "=" expression )? ";"
    node_ptr var_decl () {
        trace("var_decl");
        consume(token_types::identifier, "missing variable name in declaration");
        std::string variable_name = prev()->value_str;
        if (consume(token_types::equal_sign)) {
            return std::make_unique<DeclAndInitOperationNode>(variable_name, expression());
        }
        return std::make_unique<DeclarationOperationNode>(variable_name);
    }

    // func_decl        -> "function" IDENTIFIER "(" ( IDENTIFIER ("," IDENTIFIER)* )? ")" block
    node_ptr func_decl () {
        trace("func_decl");
        consume(token_types::identifier, "missing function name");
        std::unique_ptr<FunctionDeclNode> func_ptr = std::make_unique<FunctionDeclNode>(prev()->value_str);
        consume(token_types::round_bracket_open, "missing '(' after function name in function declaration");
        while (consume(token_types::identifier)) {
            func_ptr->add_parameter(prev()->value_str);
            if (consume(token_types::comma)) { continue; }
            break;
        }
        consume(token_types::round_bracket_close, "missing ')' after function parameters");
        func_ptr->set_body(block());
        return func_ptr;
    }

    // declaration      -> func_decl | var_decl | statement
    node_ptr declaration () {
        trace("declaration");
        if (consume(token_types::kw_function)) {
            return func_decl();
        }
        if (consume(token_types::kw_var)) {
            return var_decl();
        }
        return statement();
    }

public:
    Parser () = default;
    ~Parser () = default;

    node_ptr parse (const std::vector<Token>& tokens) {
        node_ptr main_node = std::make_unique<MainNode>();
        while (m_index < m_tokens.size()) {
            main_node->add_node(std::move(declaration()));
        }
        return main_node;
    }
};

} /* namespace mlang */