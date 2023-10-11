#include <vector>

#include "mlang/parser/parser.hpp"
#include "mlang/exception.hpp"

#include "mlang/ast/main_node.hpp"
#include "mlang/ast/block_node.hpp"
#include "mlang/ast/declaration.hpp"
#include "mlang/ast/array_node.hpp"
#include "mlang/ast/assignment.hpp"
#include "mlang/ast/value_node.hpp"
#include "mlang/ast/variable_node.hpp"
#include "mlang/ast/subscript_node.hpp"
#include "mlang/ast/binary_operations.hpp"
#include "mlang/ast/unary_operations.hpp"
#include "mlang/ast/comparison.hpp"
#include "mlang/ast/print_node.hpp"
#include "mlang/ast/func_decl_node.hpp"
#include "mlang/ast/func_call_node.hpp"
#include "mlang/ast/member_function.hpp"
#include "mlang/ast/if_node.hpp"
#include "mlang/ast/logic_operations.hpp"
#include "mlang/ast/for_node.hpp"
#include "mlang/ast/while_node.hpp"
#include "mlang/ast/break_node.hpp"
#include "mlang/ast/continue_node.hpp"
#include "mlang/ast/return_node.hpp"
#include "mlang/ast/exit_node.hpp"

#include "mlang/object/object.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {
namespace parser {

void Parser::next(int num) { m_index = std::min(m_index + num, m_tokens.size()); }
bool Parser::done() const { return m_index >= m_tokens.size(); }
const Token* Parser::peek (int num) const {
    if ((m_index + num >= 0) && (m_index + num < m_tokens.size())) {
        return m_tokens[m_index + num];
    }
    return nullptr;
}
bool Parser::peekable (int num) {
    if ((m_index + num) < 0) return false;
    if ((m_index + num) >= m_tokens.size()) return false;
    return true;
}
const Token* Parser::curr() const { return m_tokens[m_index]; }
const Token* Parser::prev() const {
    if (m_index == 0) return nullptr;
    return m_tokens[m_index - 1];
}

void Parser::trace (const std::string& str) const {
    #if TRACE_PARSER == 1
    std::cout << str << std::endl;
    #endif
}

bool Parser::consume (token_types type) {
    if (done()) { throw syntax_error{ "statement terminated unexpectedly", prev()->line, prev()->pos}; }
    if (curr()->type == type) {
        next();
        return true;
    }
    return false;
}

void Parser::consume (token_types type, const std::string& err_msg) {
    if (done()) { throw syntax_error{ err_msg, prev()->line, prev()->pos}; }
    if (curr()->type != type) {
        throw syntax_error{ err_msg, curr()->line, curr()->pos};
    }
    next();
}

// primary            -> NUMBER | STRING | "true" | "false" | "none" | "(" expression ")" | func_call | IDENTIFIER | ( "new" IDENTIFIER "(" arguments? ")" ) | ( "{" arguments "}" )
// func_call          -> IDENTIFIER "(" arguments? ")"
ast::node_ptr Parser::primary () {
    trace("primary");
    if (consume(token_types::number)) { return std::make_unique<ast::ValueNode>(object::Object{std::make_shared<object::Number>(prev()->value_num)}); }
    if (consume(token_types::string)) { return std::make_unique<ast::ValueNode>(object::Object{std::make_shared<object::String>(prev()->value_str)}); }
    if (consume(token_types::kw_true)) { return std::make_unique<ast::ValueNode>(object::Object{std::make_shared<object::Boolean>(true)}); }
    if (consume(token_types::kw_false)) { return std::make_unique<ast::ValueNode>(object::Object{std::make_shared<object::Boolean>(false)}); }
    if (consume(token_types::round_bracket_open)) {
        ast::node_ptr expr = expression();
        consume(token_types::round_bracket_close, "missing ')'");
        return expr;
    }
    if (consume(token_types::identifier)) {
        std::string identifier_str = prev()->value_str;
        if (consume(token_types::round_bracket_open)) {
            std::unique_ptr<ast::FunctionCallNode> func_call_ptr = std::make_unique<ast::FunctionCallNode>(identifier_str);
            while (!consume(token_types::round_bracket_close)) {
                func_call_ptr->add_parameter(logic_or());
            }
            return func_call_ptr;
        }
        return std::make_unique<ast::VariableNode>(identifier_str);
    }
    //if (consume(token_types::kw_new)) { return std::make_unique<VariableNode>(prev()->value_str); }
    if (consume(token_types::curly_bracket_open)) {
        std::unique_ptr<ast::ArrayNode> arr_ptr = std::make_unique<ast::ArrayNode>();
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
    throw syntax_error{ "unexpected primary token", curr()->line, curr()->pos};
    return nullptr;
}

// post_op            -> postfix_increment | postfix_decrement | ( subscript | member_access | member_call )*
// postfix_increment  -> primary "++"
// postfix_decrement  -> primary "--"
// subscript          -> primary "[" logic_or "]"
// member_access      -> primary "." IDENTIFIER
// member_call        -> primary "." IDENTIFIER "(" arguments? ")"
ast::node_ptr Parser::post_op () {
    trace("post_op");
    ast::node_ptr expr = primary();
    while (true) {
        if (consume(token_types::plus_plus)) {
            expr = std::make_unique<ast::PostfixIncrementNode>(std::move(expr));
            break;
        }
        else if (consume(token_types::dash_dash)) {
            expr = std::make_unique<ast::PostfixDecrementNode>(std::move(expr));
            break;
        }
        else if (consume(token_types::square_bracket_open)) {
            std::unique_ptr<ast::SubscriptNode> subscript_ptr = std::make_unique<ast::SubscriptNode>(std::move(expr));
            subscript_ptr->set_index(logic_or());
            consume(token_types::square_bracket_close, "missing ']' as subscript termination");
            expr = std::move(subscript_ptr);
        }
        else if (consume(token_types::dot)) {
            consume(token_types::identifier, "missing identifier in member access");
            std::string member_name = prev()->value_str;
            if (consume(token_types::round_bracket_open)) {
                std::unique_ptr<ast::MemberFunctionNode> member_func_ptr = std::make_unique<ast::MemberFunctionNode>(std::move(expr), member_name);
                while (!consume(token_types::round_bracket_close)) {
                    member_func_ptr->add_parameter(logic_or());
                }
                expr = std::move(member_func_ptr);
            }
            // TODO 
            //else {
            //    std::unique_ptr<ast::MemberAccessNode> member_access_ptr = std::make_unique<ast::MemberAccessNode>(std::move(expr), member_name);
            //    expr = std::move(member_access_ptr);
            //}
        }
        else {
            break;
        }
    }
    return expr;
}

// pre_op             -> ( "-" | "!" | "++" | "--" )? post_op;
ast::node_ptr Parser::pre_op () {
    trace("pre_op");
    if (consume(token_types::dash)) {
        ast::node_ptr rhs = post_op();
        return std::make_unique<ast::UnaryMinusOperationNode>(std::move(rhs));
    }
    else if (consume(token_types::exclamation_mark)) {
        ast::node_ptr rhs = post_op();
        return std::make_unique<ast::UnaryNotOperationNode>(std::move(rhs));
    }
    else if (consume(token_types::plus_plus)) {
        ast::node_ptr rhs = post_op();
        return std::make_unique<ast::PrefixIncrementNode>(std::move(rhs));
    }
    else if (consume(token_types::dash_dash)) {
        ast::node_ptr rhs = post_op();
        return std::make_unique<ast::PrefixDecrementNode>(std::move(rhs));
    }
    return post_op();
}

// factor             -> pre_op ( ( "/" | "*" ) pre_op )*
ast::node_ptr Parser::factor () {
    trace("factor");
    ast::node_ptr expr = pre_op();
    while (true) {
        if (consume(token_types::asterisk)) {
            ast::node_ptr rhs = pre_op();
            expr = std::make_unique<ast::BinaryArithmeticNode>(std::move(expr), std::move(rhs), ast::arithmetic_mode::mul);
        }
        else if (consume(token_types::slash)) {
            ast::node_ptr rhs = pre_op();
            expr = std::make_unique<ast::BinaryArithmeticNode>(std::move(expr), std::move(rhs), ast::arithmetic_mode::div);
        }
        else {
            break;
        }
    }
    return expr;
}

// term               -> factor ( ( "-" | "+" ) factor )*
ast::node_ptr Parser::term () {
    trace("term");
    ast::node_ptr expr = factor();
    while (true) {
        if (consume(token_types::plus)) {
            ast::node_ptr rhs = comparison();
            expr = std::make_unique<ast::BinaryArithmeticNode>(std::move(expr), std::move(rhs), ast::arithmetic_mode::add);
        }
        else if (consume(token_types::dash)) {
            ast::node_ptr rhs = comparison();
            expr = std::make_unique<ast::BinaryArithmeticNode>(std::move(expr), std::move(rhs), ast::arithmetic_mode::sub);
        }
        else {
            break;
        }
    }
    return expr;
}

// comparison         -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
ast::node_ptr Parser::comparison () {
    trace("comparison");
    ast::node_ptr expr = term();
    while (true) {
        if (consume(token_types::greater)) {
            ast::node_ptr rhs = term();
            expr = std::make_unique<ast::BinaryComparisonNode>(std::move(expr), std::move(rhs), ast::comparison_mode::greater);
        }
        else if (consume(token_types::less)) {
            ast::node_ptr rhs = term();
            expr = std::make_unique<ast::BinaryComparisonNode>(std::move(expr), std::move(rhs), ast::comparison_mode::less);
        }
        else if (consume(token_types::greater_equal)) {
            ast::node_ptr rhs = term();
            expr = std::make_unique<ast::BinaryComparisonNode>(std::move(expr), std::move(rhs), ast::comparison_mode::greater_equal);
        }
        else if (consume(token_types::less_equal)) {
            ast::node_ptr rhs = term();
            expr = std::make_unique<ast::BinaryComparisonNode>(std::move(expr), std::move(rhs), ast::comparison_mode::less_equal);
        }
        else {
            break;
        }
    }
    return expr;
}

// equality           -> comparison ( ( "!=" | "==" ) comparison )*
ast::node_ptr Parser::equality () {
    trace("equality");
    ast::node_ptr expr = comparison();
    while (true) {
        if (consume(token_types::exclamation_equal)) {
            ast::node_ptr rhs = comparison();
            expr = std::make_unique<ast::BinaryComparisonNode>(std::move(expr), std::move(rhs), ast::comparison_mode::not_equal);
        }
        else if (consume(token_types::double_equal)) {
            ast::node_ptr rhs = comparison();
            expr = std::make_unique<ast::BinaryComparisonNode>(std::move(expr), std::move(rhs), ast::comparison_mode::equal);
        }
        else {
            break;
        }
    }
    return expr;
}

// logic_and          -> equality ( "&&" equality )*
ast::node_ptr Parser::logic_and () {
    trace("logic_and");
    ast::node_ptr expr = equality();
    while (consume(token_types::double_ampersand)) {
        ast::node_ptr rhs = equality();
        expr = std::make_unique<ast::BinaryLogicNode>(std::move(expr), std::move(rhs), ast::logic_mode::logic_and);
    }
    return expr;
}

// logic_or           -> logic_and ( "||" logic_and )*
ast::node_ptr Parser::logic_or () {
    trace("logic_or");
    ast::node_ptr expr = logic_and();
    while (consume(token_types::double_pipe)) {
        ast::node_ptr rhs = logic_and();
        expr = std::make_unique<ast::BinaryLogicNode>(std::move(expr), std::move(rhs), ast::logic_mode::logic_or);
    }
    return expr;
}

// assignment         -> logic_or ( "=" | "+=" | "-=" | "*=" | "/=" logic_or )?
ast::node_ptr Parser::assignment () {
    trace("assignment");
    ast::node_ptr expr = logic_or();
    if (consume(token_types::equal_sign)) {
        ast::node_ptr rhs = logic_or();
        return std::make_unique<ast::AssignmentNode>(std::move(expr), std::move(rhs), ast::assignment_mode::simple);
    }
    if (consume(token_types::plus_equal)) {
        ast::node_ptr rhs = logic_or();
        return std::make_unique<ast::AssignmentNode>(std::move(expr), std::move(rhs), ast::assignment_mode::add);
    }
    if (consume(token_types::dash_equal)) {
        ast::node_ptr rhs = logic_or();
        return std::make_unique<ast::AssignmentNode>(std::move(expr), std::move(rhs), ast::assignment_mode::sub);
    }
    if (consume(token_types::asterisk_equal)) {
        ast::node_ptr rhs = logic_or();
        return std::make_unique<ast::AssignmentNode>(std::move(expr), std::move(rhs), ast::assignment_mode::mul);
    }
    if (consume(token_types::slash_equal)) {
        ast::node_ptr rhs = logic_or();
        return std::make_unique<ast::AssignmentNode>(std::move(expr), std::move(rhs), ast::assignment_mode::div);
    }
    return expr;
}

// expression         -> assignment
ast::node_ptr Parser::expression () {
    trace("expression");
    return assignment();
}

// break_statement    -> "break" ";"
ast::node_ptr Parser::break_statement () {
    trace("break_statement");
    consume(token_types::semicolon, "missing ';' as 'break' statement termination");
    return std::make_unique<ast::BreakNode>();
}

// continue_statement -> "continue" ";"
ast::node_ptr Parser::continue_statement () {
    trace("continue_statement");
    consume(token_types::semicolon, "missing ';' as 'continue' statement termination");
    return std::make_unique<ast::ContinueNode>();
}

// return_statement   -> "return" expression? ";"
ast::node_ptr Parser::return_statement () {
    trace("return_statement");
    ast::node_ptr value = expression();
    /* TODO : optional return value */
    consume(token_types::semicolon, "missing ';' as 'return' statement termination");
    return std::make_unique<ast::ReturnNode>(std::move(value));
}

// exit_statement     -> "exit" expression ";"
ast::node_ptr Parser::exit_statement () {
    trace("exit_statement");
    ast::node_ptr value = expression();
    consume(token_types::semicolon, "missing ';' as 'exit' statement termination");
    return std::make_unique<ast::ExitNode>(std::move(value));
}

// print_statement  -> "print" "(" STRING ( "," expression )* ")" ";"
ast::node_ptr Parser::print_statement () {
    trace("print_statement");
    std::unique_ptr<ast::PrintNode> print_node_ptr = std::make_unique<ast::PrintNode>();
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
ast::node_ptr Parser::while_statement () {
    trace("while_statement");
    std::unique_ptr<ast::WhileStatementNode> while_node_ptr = std::make_unique<ast::WhileStatementNode>();
    consume(token_types::round_bracket_open, "missing '(' after 'while'");
    while_node_ptr->set_condition(expression());
    consume(token_types::round_bracket_close, "missing ')' after 'while' statement condition");
    while_node_ptr->set_body(block());
    return while_node_ptr;
}

// for_statement    -> "for" "(" ( var_decl | exp_statement | ";" ) expression? ";" expression? ")" block
ast::node_ptr Parser::for_statement () {
    trace("for_statement");
    std::unique_ptr<ast::ForStatementNode> for_node_ptr = std::make_unique<ast::ForStatementNode>();
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
    return for_node_ptr;
}

// if_statement     -> "if" "(" expression ")" block ( "elif" "(" expression ")" block )* ( "else" block )?
ast::node_ptr Parser::if_statement () {
    trace("if_statement");
    std::unique_ptr<ast::IfStatementNode> if_node_ptr = std::make_unique<ast::IfStatementNode>();
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
ast::node_ptr Parser::exp_statement () {
    trace("exp_statement");
    ast::node_ptr exp = expression();
    consume(token_types::semicolon, "missing ';' at expression termination");
    return exp;
}

// statement        -> exp_statement | if_statement | for_statement | while_statement | print_statement | control
ast::node_ptr Parser::statement () {
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
ast::node_ptr Parser::block () {
    trace("block");
    consume(token_types::curly_bracket_open, "missing scope opening '{' token");
    std::unique_ptr<ast::BlockNode> block_ptr = std::make_unique<ast::BlockNode>();
    while (!consume(token_types::curly_bracket_close)) {
        block_ptr->add_node(statement());
    }
    return block_ptr;
}

// var_decl         -> "var" IDENTIFIER ( "=" expression )? ";"
ast::node_ptr Parser::var_decl () {
    trace("var_decl");
    consume(token_types::identifier, "missing variable name in declaration");
    std::string variable_name = prev()->value_str;
    if (consume(token_types::equal_sign)) {
        ast::node_ptr exp = expression();
        consume(token_types::semicolon, "missing ';' declaration termination");
        return std::make_unique<ast::DeclAndInitOperationNode>(variable_name, std::move(exp));
    }
    consume(token_types::semicolon, "missing ';' declaration termination");
    return std::make_unique<ast::DeclarationOperationNode>(variable_name);
}

// func_decl        -> "function" IDENTIFIER "(" ( IDENTIFIER ("," IDENTIFIER)* )? ")" block
ast::node_ptr Parser::func_decl () {
    trace("func_decl");
    consume(token_types::identifier, "missing function name");
    std::unique_ptr<ast::FunctionDeclNode> func_ptr = std::make_unique<ast::FunctionDeclNode>(prev()->value_str);
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
ast::node_ptr Parser::declaration () {
    trace("declaration");
    if (consume(token_types::kw_function)) {
        return func_decl();
    }
    if (consume(token_types::kw_var)) {
        return var_decl();
    }
    return statement();
}


ast::node_ptr Parser::parse (const std::vector<Token>& tokens) {
    for (const Token& token : tokens) {
        m_tokens.push_back(&token);
    }
    std::unique_ptr<ast::MainNode> main_node = std::make_unique<ast::MainNode>();
    while (m_index < m_tokens.size()) {
        main_node->add_node(declaration());
    }
    return main_node;
}

} /* namespace parser */
} /* namespace mlang */