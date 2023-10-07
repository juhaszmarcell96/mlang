#pragma once

#include <vector>

#include "mlang/token.hpp"
#include "mlang/environment.hpp"
#include "mlang/exception.hpp"
#include "mlang/parser/ast.hpp"

#include "mlang/object/object.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/number.hpp"

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

    void assert_current (token_types type, const std::string& err_msg) const {
        if (done()) { throw syntax_error{ err_msg, prev()->line, prev()->pos}; }
        if (curr()->type != type) {
            throw syntax_error{ err_msg, curr()->line, curr()->pos};
        }
    }

    void assert_next (const std::string& err_msg) {
        next();
        if (done()) { throw syntax_error{ err_msg, prev()->line, prev()->pos}; }
    }

    void assert_next (token_types type, const std::string& err_msg) {
        next();
        if (done()) { throw syntax_error{ err_msg, prev()->line, prev()->pos}; }
        if (curr()->type != type) {
            throw syntax_error{ err_msg, curr()->line, curr()->pos};
        }
    }

    void assert_next_done () {
        next();
        if (!done()) {
            const Token* token_ptr = prev();
            if (token_ptr) {
                throw syntax_error{ "expression not terminated properly", token_ptr->line, token_ptr->pos};
            }
            else {
                throw syntax_error{ "expression not terminated properly", curr()->line, curr()->pos};
            }
        }
    }

    void assert_done () {
        if (!done()) {
            const Token* token_ptr = prev();
            if (token_ptr) {
                throw syntax_error{ "expression not terminated properly", token_ptr->line, token_ptr->pos};
            }
            else {
                throw syntax_error{ "expression not terminated properly", curr()->line, curr()->pos};
            }
        }
    }

    void assert_not_done () {
        if (done()) { throw syntax_error{ "expression terminated unexpectedly", prev()->line, prev()->pos}; }
    }

    bool check_type (token_types type) {
        if (done()) { throw syntax_error{ "expression terminated unexpectedly", prev()->line, prev()->pos}; }
        return curr()->type == type;
    }

    void assert_error (const std::string& err_msg) {
        throw syntax_error{ err_msg, curr()->line, curr()->pos};
    }

    node_ptr statement () {
        trace("statement");
        token_types current_type = curr()->type;
        switch (current_type) {
            case token_types::kw_var: {
                return declaration();
            }
            case token_types::kw_if: {
                return if_statement();
            }
            case token_types::kw_elif: {
                elif_statement();
                return nullptr;
            }
            case token_types::kw_else: {
                else_statement();
                return nullptr;
            }
            case token_types::kw_for: {
                return for_statement();
            }
            case token_types::kw_while: {
                return while_statement();
            }
            case token_types::kw_break: {
                return break_statement();
            }
            case token_types::kw_return: {
                return return_statement();
            }
            case token_types::kw_end: {
                return end_statement(); /* can return nullptr as well if it does not do anything */
            }
            case token_types::kw_function: {
                /* function declaration is added to the current scope, the AST node execute adds the function to the environment */
                /* and it does nothing else, does not execute the body. The function object in the environment has a pointer to  */
                /* the function AST node and it calls a distinct "call" function to execute the body in a given context */
                return function_declaration();
            }
            case token_types::identifier: {
                if (!peekable()) return primary();
                token_types next_type = peek()->type;
                switch (next_type) {
                    case token_types::asterisk_equal:
                    case token_types::slash_equal:
                    case token_types::dash_equal:
                    case token_types::plus_equal:
                    case token_types::equal_sign: {
                        return assignment();
                    }
                    //case token_types::round_bracket_open: {
                    //    return function_call();
                    //}
                    default: {
                        return expression();
                    }
                }
                break;
            }
            case token_types::kw_print: {
                return print_expr();
            }
            default : {
                return expression();
            }
        }
    }

    node_ptr assignment () {
        trace("assignment");
        if (curr()->type != token_types::identifier) {
            throw unexpected_error{"statement not an assignment"};
        }
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        switch (curr()->type) {
            case token_types::equal_sign : {
                trace("simple assignment");
                next();
                node_ptr rhs = expression();
                return std::make_unique<AssignmentOperationNode>(variable_name, std::move(rhs));
            }
            case token_types::plus_equal : {
                trace("plus assignment");
                next();
                node_ptr rhs = expression();
                return std::make_unique<AddEqualOperationNode>(variable_name, std::move(rhs));
            }
            case token_types::dash_equal : {
                trace("minus assignment");
                next();
                node_ptr rhs = expression();
                return std::make_unique<SubEqualOperationNode>(variable_name, std::move(rhs));
            }
            case token_types::asterisk_equal : {
                trace("multiply assignment");
                next();
                node_ptr rhs = expression();
                return std::make_unique<MulEqualOperationNode>(variable_name, std::move(rhs));
            }
            case token_types::slash_equal : {
                trace("divide assignment");
                next();
                node_ptr rhs = expression();
                return std::make_unique<DivEqualOperationNode>(variable_name, std::move(rhs));
            }
            default : {
                throw unexpected_error{"statement not an assignment, no assignment operator found"};
            }
        }
        throw unexpected_error{"unexpected error in assignment"};
        return nullptr;
    }

    node_ptr declaration () {
        // declaration -> "var" IDENTIFIER ( "=" expression )?
        trace("declaration");
        assert_next(token_types::identifier, "missing variable name in declaration");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) {
            return std::make_unique<DeclarationOperationNode>(variable_name);
        }
        if (curr()->type == token_types::equal_sign) {
            assert_next("statement terminated unexpectedly");
            node_ptr rhs = expression();
            assert_done();
            return std::make_unique<DeclAndInitOperationNode>(variable_name, std::move(rhs));
        }
        throw syntax_error{ "invalid token found in declaration", curr()->line, curr()->pos };
        return nullptr;
    }

    node_ptr print_expr () {
        // print       -> "print" "(" expression ")"
        trace("print_expr");
        assert_current(token_types::kw_print, "statement not a 'print' statement");
        assert_next(token_types::round_bracket_open, "missing '(' after 'print'");
        assert_next(token_types::string, "first parameter of 'print' must be a string");
        std::string rule = curr()->value_str;
        next();
        std::vector<node_ptr> args;
        while (true) {
            if (done()) { throw syntax_error{ "missing ')'", curr()->line, curr()->pos}; }
            if (curr()->type == token_types::round_bracket_close) {
                break;
            }
            else if (curr()->type == token_types::comma) {
                next();
                args.push_back(expression());
            }
            else {
                throw syntax_error{ "unexpected token in 'print'", curr()->line, curr()->pos};
            }
        }
        return std::make_unique<PrintNode>(rule, std::move(args));
    }

    node_ptr function_declaration () {
        // func_decl   -> "function" IDENTIFIER "(" ( IDENTIFIER IDENTIFIER ("," IDENTIFIER IDENTIFIER)* )? ")" "-" ">" IDENTIFIER;
        trace("function_declaration");
        consume(token_types::kw_function, "statement not a function declaration");
        consume(token_types::identifier, "missing function name");
        std::unique_ptr<FunctionDeclNode> func_ptr = std::make_unique<FunctionDeclNode>(prev()->value_str, m_current_scope);
        consume(token_types::round_bracket_open, "missing '(' after keyword 'function'");
        while (true) {
            if (consume(token_types::identifier)) {
                std::string type_name = prev()->value_str;
                consume(token_types::identifier, "missing identifier after typename '" + type_name + "'");
                func_ptr->add_parameter(type_name, prev()->value_str);
            }

            if (consume(token_types::comma)) { continue; }
            
            consume(token_types::round_bracket_close, "missing ')' after function declaration");
            break;
        }
        consume(token_types::dash, "missing return type after function declaration");
        consume(token_types::greater, "missing return type after function declaration");
        consume(token_types::identifier, "invalid return type");
        func_ptr->define_ret_type(prev()->value_str);
        assert_done();
        m_next_scope = func_ptr.get();
        return func_ptr;
    }

    node_ptr if_statement () {
        // if          -> "if" "(" expression ")"
        trace("if_statement");
        assert_current(token_types::kw_if, "statement not an 'if' statement");
        assert_next(token_types::round_bracket_open, "missing '(' after 'if'");
        assert_next("condition not found for 'if'");
        node_ptr condition = expression();
        assert_current(token_types::round_bracket_close, "unmatched parentheses");
        assert_next_done();
        std::unique_ptr<IfStatementNode> if_node_ptr = std::make_unique<IfStatementNode>(std::move(condition), m_current_scope);
        m_next_scope = if_node_ptr.get();
        return if_node_ptr;
    }

    void elif_statement () {
        // elif        -> "elif" "(" expression ")"
        trace("elif_statement");
        assert_current(token_types::kw_elif, "statement not an 'elif' statement");
        assert_next(token_types::round_bracket_open, "missing '(' after 'elif'");
        assert_next("condition not found for 'elif'");
        node_ptr condition = expression();
        assert_current(token_types::round_bracket_close, "unmatched parentheses");
        assert_next_done();
        m_current_scope->add_elif(std::move(condition));
    }

    void else_statement () {
        // else        -> "else"
        trace("else_statement");
        assert_current(token_types::kw_else, "statement not an 'else' statement");
        assert_next_done();
        m_current_scope->add_else();
    }

    node_ptr for_statement () {
        // for         -> "for" "(" assignment ( "," assignment )* ";" expression ( "," expression )* ";" expression ( "," expression )* ")"
        trace("for_statement");
        assert_current(token_types::kw_for, "statement not a 'for' statement");
        assert_next(token_types::round_bracket_open, "missing '(' after 'for'");
        std::unique_ptr<ForStatementNode> for_node_ptr = std::make_unique<ForStatementNode>(m_current_scope);
        /* processing assignments */
        assert_next("assignments not found for 'for'");
        while (true) {
            if (check_type(token_types::semicolon)) { break; }
            for_node_ptr->add_assignment(std::move(assignment()));
            if (check_type(token_types::semicolon)) { break; }
            else if (check_type(token_types::comma)) { continue; }
            assert_error("unexpected token in 'for' initialization");
        }
        /* processing tests */
        assert_next("tests not found for 'for'");
        while (true) {
            if (check_type(token_types::semicolon)) { break; }
            for_node_ptr->add_test(std::move(expression()));
            if (check_type(token_types::semicolon)) { break; }
            else if (check_type(token_types::comma)) { continue; }
            assert_error("unexpected token in 'for' tests");
        }
        /* processing updates */
        assert_next("updates not found for 'for'");
        while (true) {
            if (check_type(token_types::round_bracket_close)) { break; }
            for_node_ptr->add_update(std::move(assignment()));
            if (check_type(token_types::round_bracket_close)) { break; }
            else if (check_type(token_types::comma)) { continue; }
            assert_error("unexpected token in 'for' updates");
        }
        assert_next_done();
        m_next_scope = for_node_ptr.get();
        return for_node_ptr;
    }

    node_ptr while_statement () {
        // while       -> "while" "(" expression ")"
        trace("while_statement");
        assert_current(token_types::kw_while, "statement not a 'while' statement");
        assert_next(token_types::round_bracket_open, "missing '(' after 'while'");
        assert_next("condition not found for 'while'");
        node_ptr condition = expression();
        assert_current(token_types::round_bracket_close, "unmatched parentheses");
        assert_next_done();
        std::unique_ptr<WhileStatementNode> while_node_ptr = std::make_unique<WhileStatementNode>(std::move(condition), m_current_scope);
        m_next_scope = while_node_ptr.get();
        return while_node_ptr;
    }

    node_ptr end_statement () {
        /* endif       -> "endif" */
        trace("end_statement");
        assert_current(token_types::kw_end, "statement not an 'end' statement");
        assert_next_done();
        m_next_scope = m_current_scope->get_parent();
        return std::make_unique<EndStatementNode>();
    }

    node_ptr break_statement () {
        trace("break_statement");
        assert_current(token_types::kw_break, "statement not a 'break' statement");
        assert_next_done();
        return std::make_unique<BreakNode>();
    }

    node_ptr return_statement () {
        trace("return_statement");
        consume(token_types::kw_return, "not a return statement");
        node_ptr value = expression();
        assert_done();
        return std::make_unique<ReturnNode>(std::move(value));
    }

    node_ptr expression () {
        // expression -> equality
        trace("expression");
        return equality();
    }

    node_ptr equality () {
        // equality   -> comparison ( ( "!=" | "==" ) comparison )* ;
        trace("equality");
        node_ptr expr = comparison();
        if (done()) return expr;
        while (check_type(token_types::double_equal) || check_type(token_types::exclamation_equal)) {
            token_types current_type = curr()->type;
            assert_next("statement terminated unexpectedly");
            node_ptr rhs = comparison();
            if (current_type == token_types::double_equal) {
                expr = std::make_unique<BinaryEqualityOperationNode>(std::move(expr), std::move(rhs));
            }
            else {
                expr = std::make_unique<BinaryInequalityOperationNode>(std::move(expr), std::move(rhs));
            }
            if (done()) return expr;
        }
        return expr;
    }

    node_ptr comparison () {
        // comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        trace("comparison");
        node_ptr expr = term();
        if (done()) return expr;
        while (check_type(token_types::greater) || check_type(token_types::less) ||
               check_type(token_types::greater_equal) || check_type(token_types::less_equal)) {
            token_types current_type = curr()->type;
            assert_next("statement terminated unexpectedly");
            node_ptr rhs = term();
            if (current_type == token_types::greater) {
                expr = std::make_unique<ComparisonGreaterNode>(std::move(expr), std::move(rhs));
            }
            else if (current_type == token_types::less) {
                expr = std::make_unique<ComparisonLessNode>(std::move(expr), std::move(rhs));
            }
            else if (current_type == token_types::greater_equal) {
                expr = std::make_unique<ComparisonGreaterEqualNode>(std::move(expr), std::move(rhs));
            }
            else {
                expr = std::make_unique<ComparisonLessEqualNode>(std::move(expr), std::move(rhs));
            }
            if (done()) return expr;
        }
        return expr;
    }

    node_ptr term () {
        // term       -> factor ( ( "-" | "+" ) factor )* ;
        trace("term");
        node_ptr expr = factor();
        if (done()) return expr;
        while (check_type(token_types::plus) || check_type(token_types::dash)) {
            token_types current_type = curr()->type;
            assert_next("statement terminated unexpectedly");
            node_ptr rhs = factor();
            if (current_type == token_types::plus) {
                expr = std::make_unique<BinaryAddOperationNode>(std::move(expr), std::move(rhs));
            }
            else {
                expr = std::make_unique<BinarySubOperationNode>(std::move(expr), std::move(rhs));
            }
            if (done()) return expr;
        }
        return expr;
    }

    node_ptr factor () {
        // factor     -> unary ( ( "/" | "*" ) unary )* ;
        trace("factor");
        node_ptr expr = unary();
        if (done()) return expr;
        while (check_type(token_types::asterisk) || check_type(token_types::slash)) {
            token_types current_type = curr()->type;
            assert_next("statement terminated unexpectedly");
            node_ptr rhs = unary();
            if (current_type == token_types::asterisk) {
                expr = std::make_unique<BinaryMulOperationNode>(std::move(expr), std::move(rhs));
            }
            else {
                expr = std::make_unique<BinaryDivOperationNode>(std::move(expr), std::move(rhs));
            }
            if (done()) return expr;
        }
        return expr;
    }

    node_ptr unary () {
        // unary      -> ( "-" | "!" ) unary | primary;
        trace("unary");
        assert_not_done();
        if (check_type(token_types::dash) || check_type(token_types::exclamation_mark)) {
            token_types current_type = curr()->type;
            assert_next("statement terminated unexpectedly");
            node_ptr rhs = unary();
            if (current_type == token_types::exclamation_mark) {
                return std::make_unique<UnaryNotOperationNode>(std::move(rhs));
            }
            else {
                return std::make_unique<UnaryMinusOperationNode>(std::move(rhs));
            }
        }
        return primary();
    }

    node_ptr primary () {
        // primary    -> NUMBER | STRING | "true" | "false" | "(" expression ")" | IDENTIFIER;
        trace("primary");
        assert_not_done();
        if (check_type(token_types::number)) {
            trace("primary number");
            double current_value = curr()->value_num;
            next();
            return std::make_unique<ValueNode>(std::make_shared<Number>(current_value));
        }
        if (check_type(token_types::string)) {
            trace("primary string");
            std::string current_value = curr()->value_str;
            next();
            return std::make_unique<ValueNode>(std::make_shared<String>(current_value));
        }
        //if (check_type(token_types::kw_true)) {
        //    trace("primary true");
        //    next();
        //    return std::make_unique<ValueNode>(Value{true});
        //}
        //if (check_type(token_types::kw_false)) {
        //    trace("primary false");
        //    next();
        //    return std::make_unique<ValueNode>(Value{false});
        //}
        if (check_type(token_types::round_bracket_open)) {
            trace("primary '('");
            assert_next("statement terminated unexpectedly");
            node_ptr expr = expression();
            assert_current(token_types::round_bracket_close, "unmatched parentheses");
            next();
            return expr;
        }
        if (check_type(token_types::identifier)) {
            trace("primary identifier");
            std::string current_str = curr()->value_str;
            next();
            if (!done()) {
                if (check_type(token_types::round_bracket_open)) {
                    return function_call(current_str);
                }
                if (check_type(token_types::square_bracket_open)) {
                    return indexing(current_str);
                }
            }
            return std::make_unique<VariableNode>(current_str);
        }
        throw syntax_error{ "unexpected primary token", curr()->line, curr()->pos};
        return nullptr;
    }

    node_ptr function_call (const std::string& name) {
        // func_call   -> IDENTIFIER "(" (expression)* ")"
        trace("function_call");
        consume(token_types::round_bracket_open, "missing '(' in function call");
        std::unique_ptr<FunctionCallNode> func_ptr = std::make_unique<FunctionCallNode>(name);
        while (true) {
            if (consume(token_types::round_bracket_close)) { break; }
            func_ptr->add_parameter(expression());
            if (consume(token_types::comma)) { continue; }
            if (consume(token_types::round_bracket_close)) { break; }
            assert_error("invalid function call syntax");
        }
        assert_done();
        return func_ptr;
    }

    node_ptr indexing (const std::string& name) {
        trace("indexing");
        consume(token_types::square_bracket_open, "missing '[' in function call");
        node_ptr index_expr = expression();
        consume(token_types::square_bracket_close, "missing ']' in function call");
        return std::make_unique<IndexingNode>(name, std::move(index_expr));
    }

public:
    Parser () = default;
    ~Parser () = default;

    node_ptr parse (const std::vector<Token>& tokens) {
        node_ptr main_node = std::make_unique<MainNode>();
        m_current_scope = main_node.get();
        m_next_scope = main_node.get();
        int skip_semicolons { 0 };
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i].type != token_types::semicolon) {
                m_tokens.push_back(&(tokens[i]));
                if (tokens[i].type == token_types::kw_for) { skip_semicolons = 2; }
            }
            else if ((tokens[i].type == token_types::semicolon) && skip_semicolons != 0) {
                m_tokens.push_back(&(tokens[i]));
                --skip_semicolons;
            }
            else {
                trace("###########################################");
                node_ptr statement_ptr = statement();
                if (statement_ptr != nullptr) { m_current_scope->add_node(std::move(statement_ptr)); }
                if (m_current_scope != m_next_scope) {
                    m_current_scope = m_next_scope;
                }
                m_tokens.clear();
                m_index = 0;
            }
        }
        if (m_tokens.size() != 0) {
            /* TODO : last line is not terminated with ';' */
            throw syntax_error{ "missing ';'", m_tokens.back()->line, m_tokens.back()->pos};
        }
        return main_node;
    }
};

} /* namespace mlang */