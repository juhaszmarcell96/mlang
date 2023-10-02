#pragma once

#include <vector>

#include "mlang/token.hpp"
#include "mlang/environment.hpp"
#include "mlang/value.hpp"
#include "mlang/exception.hpp"
#include "mlang/parser/ast.hpp"

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

    void trace (const std::string& str) const {
        #if TRACE_PARSER == 1
        std::cout << str << std::endl;
        #endif
    }

    node_ptr expression() {
        trace("expression");
        trace("index = " + std::to_string(m_index));
        /* the highest level parsing function calls the function for the lowest precedence operation */
        token_types current_type = curr()->type;
        switch (current_type) {
            case token_types::kw_string:
            case token_types::kw_number:
            case token_types::kw_array:
            case token_types::kw_bool: {
                return declaration();
            }
            case token_types::kw_if: {
                return if_statement();
            }
            case token_types::kw_endif: {
                return endif_statement();
            }
            case token_types::round_bracket_open: {
                return primary();
            }
            case token_types::identifier: {
                if (!peekable()) return primary();
                token_types next_type = peek()->type;
                switch (next_type) {
                    case token_types::equal_sign: {
                        return assignment();
                    }
                    case token_types::plus_equal: {
                        return add_equal();
                    }
                    case token_types::dash_equal: {
                        return sub_equal();
                    }
                    case token_types::slash_equal: {
                        return div_equal();
                    }
                    case token_types::asterisk_equal: {
                        return mul_equal();
                    }
                    case token_types::double_equal: {
                        return equality_check();
                    }
                    case token_types::exclamation_equal: {
                        return inequality_check();
                    }
                    //case token_types::round_bracket_open: {
                    //    return function_call();
                    //}
                    default: {
                        return add_sub();
                    }
                }
                break;
            }
            case token_types::number : {
                return add_sub();
            }
            default : {
                return nullptr;
            }
        }
    }

    node_ptr equality_check () {
        trace("equality_check");
        node_ptr lhs = add_sub(); // left-hand side of the equality
        if (done() || curr()->type != token_types::double_equal) return lhs; // not an equality check
        next();
        node_ptr rhs = add_sub(); // right-hand side of the equality
        return std::make_unique<BinaryEqualityOperationNode>(std::move(lhs), std::move(rhs));
    }

    node_ptr inequality_check () {
        trace("inequality_check");
        node_ptr lhs = add_sub(); // left-hand side of the equality
        if (done() || curr()->type != token_types::exclamation_equal) return lhs; // not an inequality check
        next();
        node_ptr rhs = add_sub(); // right-hand side of the equality
        return std::make_unique<BinaryInequalityOperationNode>(std::move(lhs), std::move(rhs));
    }

    node_ptr if_statement () {
        trace("if_statement");
        next();
        if (done()) { throw syntax_error{ "condition not found for 'if'", curr()->line, curr()->pos}; }
        node_ptr condition = expression();
        std::unique_ptr<IfStatementNode> node_ptr = std::make_unique<IfStatementNode>(std::move(condition), m_current_scope);
        m_current_scope = node_ptr.get();
        return node_ptr;
    }

    node_ptr endif_statement () {
        trace("endif_statement");
        m_current_scope = m_current_scope->get_parent();
        return std::make_unique<EndifStatementNode>();
    }

    node_ptr add_equal () {
        trace("add_equal");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::plus_equal) { throw syntax_error{ "this is not '+='", curr()->line, curr()->pos}; }
        next();
        node_ptr rhs = expression();
        return std::make_unique<AddEqualOperationNode>(variable_name, std::move(rhs));
    }

    node_ptr sub_equal () {
        trace("sub_equal");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::dash_equal) { throw syntax_error{ "this is not '-='", curr()->line, curr()->pos}; }
        next();
        node_ptr rhs = expression();
        return std::make_unique<SubEqualOperationNode>(variable_name, std::move(rhs));
    }

    node_ptr mul_equal () {
        trace("mul_equal");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::asterisk_equal) { throw syntax_error{ "this is not '*='", curr()->line, curr()->pos}; }
        next();
        node_ptr rhs = expression();
        return std::make_unique<MulEqualOperationNode>(variable_name, std::move(rhs));
    }

    node_ptr div_equal () {
        trace("div_equal");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::slash_equal) { throw syntax_error{ "this is not '/='", curr()->line, curr()->pos}; }
        next();
        node_ptr rhs = expression();
        return std::make_unique<DivEqualOperationNode>(variable_name, std::move(rhs));
    }

    node_ptr declaration () {
        trace("declaration");
        value_types variable_type;
        switch (curr()->type) {
            case token_types::kw_array  : { variable_type = value_types::array; break; }
            case token_types::kw_number : { variable_type = value_types::number; break; }
            case token_types::kw_string : { variable_type = value_types::string; break; }
            case token_types::kw_bool   : { variable_type = value_types::boolean; break; }
            default : { throw bad_type_error {}; break; }
        }
        next();
        if (done()) { throw syntax_error{ "did not find variable name", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::identifier) { throw syntax_error{ "variable name must be an identifier", curr()->line, curr()->pos}; }
        std::string variable_name = curr()->value_str;
        next();
        if (!done()) { throw syntax_error{ "declaration must be closed with ';'", curr()->line, curr()->pos}; }
        return std::make_unique<DeclarationOperationNode>(variable_type, variable_name);
    }

    node_ptr assignment () {
        trace("assignment");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::equal_sign) { throw syntax_error{ "this is not an assignment", curr()->line, curr()->pos}; }
        next();
        node_ptr rhs = expression();
        return std::make_unique<AssignmentOperationNode>(variable_name, std::move(rhs));
    }

    node_ptr add_sub () {
        trace("add_sub");
        /* withing an addition, we can have a multiplication */
        node_ptr expr = mul_div();
        if (done()) return expr;
        while (curr()->type == token_types::plus || curr()->type == token_types::dash) {
            token_types current_type = curr()->type;
            next();
            node_ptr rhs = mul_div();
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

    node_ptr mul_div () {
        trace("mul_div");
        node_ptr expr = unary();
        if (done()) return expr;
        while (curr()->type == token_types::asterisk || curr()->type == token_types::slash) {
            token_types current_type = curr()->type;
            next();
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
        trace("unary");
        // for now, let's just return a primary.
        /* TODO : implement ++ and -- */
        return primary();
    }

    node_ptr primary () {
        trace("primary");
        if (curr()->type == token_types::number) {
            double current_value = curr()->value_num;
            next();
            return std::make_unique<ValueNode>(Value{current_value});
        }
        if (curr()->type == token_types::round_bracket_open) {
            next();
            node_ptr expr = expression();
            if (curr()->type != token_types::round_bracket_close) {
                throw syntax_error{ "unmatched parentheses", curr()->line, curr()->pos};
            }
            next();
            return expr;
        }
        if (curr()->type == token_types::identifier) {
            std::string current_str = curr()->value_str;
            next();
            return std::make_unique<VariableNode>(current_str);
        }
        throw syntax_error{ "unexpected primary token", curr()->line, curr()->pos};
        return nullptr;
    }
public:
    Parser () = default;
    ~Parser () = default;

    node_ptr parse (const std::vector<Token>& tokens) {
        node_ptr main_node = std::make_unique<MainNode>();
        m_current_scope = main_node.get();
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i].type != token_types::semicolon) {
                m_tokens.push_back(&(tokens[i]));
            }
            else {
                m_current_scope->add_node(expression());
                m_tokens.clear();
                m_index = 0;
            }
        }
        return main_node;
    }
};

} /* namespace mlang */