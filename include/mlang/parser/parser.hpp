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

    node_ptr statement () {
        trace("statement");
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
            case token_types::kw_end: {
                return end_statement();
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
        // declaration -> "number" | "string" | "array" | "bool" IDENTIFIER ( "=" expression )?
        trace("declaration");
        value_types variable_type;
        switch (curr()->type) {
            case token_types::kw_array  : { variable_type = value_types::array; break; }
            case token_types::kw_number : { variable_type = value_types::number; break; }
            case token_types::kw_string : { variable_type = value_types::string; break; }
            case token_types::kw_bool   : { variable_type = value_types::boolean; break; }
            default : { throw unexpected_error{"type name not found in declaration"}; break; }
        }
        next();
        if (done()) { throw syntax_error{ "did not find variable name", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::identifier) { throw syntax_error{ "variable name must be an identifier", curr()->line, curr()->pos}; }
        std::string variable_name = curr()->value_str;
        /* TODO : assignment -> overload declaration constructor */
        return std::make_unique<DeclarationOperationNode>(variable_type, variable_name);
    }

    node_ptr if_statement () {
        // if          -> "if" "(" expression ")"
        trace("if_statement");
        if (curr()->type != token_types::kw_if) {
            throw unexpected_error{"statement not an if statement"};
        }
        next();
        if (done()) { throw syntax_error{ "condition not found for 'if'", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::round_bracket_open) {
            throw syntax_error{ "missing '(' after 'if'", curr()->line, curr()->pos};
        }
        next();
        if (done()) { throw syntax_error{ "condition not found for 'if'", curr()->line, curr()->pos}; }
        node_ptr condition = expression();
        if (curr()->type != token_types::round_bracket_close) {
            throw syntax_error{ "unmatched parentheses", curr()->line, curr()->pos};
        }
        std::unique_ptr<IfStatementNode> node_ptr = std::make_unique<IfStatementNode>(std::move(condition), m_current_scope);
        m_current_scope = node_ptr.get();
        return node_ptr;
    }

    node_ptr end_statement () {
        /* endif       -> "endif" */
        trace("end_statement");
        if (curr()->type != token_types::kw_end) {
            throw unexpected_error{"statement not an if statement"};
        }
        m_current_scope = m_current_scope->get_parent();
        return std::make_unique<EndifStatementNode>();
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
        while (curr()->type == token_types::double_equal || curr()->type == token_types::exclamation_equal) {
            token_types current_type = curr()->type;
            next();
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
        while (curr()->type == token_types::greater || curr()->type == token_types::less ||
               curr()->type == token_types::greater_equal || curr()->type == token_types::less_equal) {
            token_types current_type = curr()->type;
            next();
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
        while (curr()->type == token_types::plus || curr()->type == token_types::dash) {
            token_types current_type = curr()->type;
            next();
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
        // unary      -> ( "-" | "!" ) unary | primary;
        trace("unary");
        if (curr()->type == token_types::dash || curr()->type == token_types::exclamation_mark) {
            token_types current_type = curr()->type;
            next();
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
        if (curr()->type == token_types::number) {
            trace("primary number");
            double current_value = curr()->value_num;
            next();
            return std::make_unique<ValueNode>(Value{current_value});
        }
        if (curr()->type == token_types::string) {
            trace("primary string");
            std::string current_value = curr()->value_str;
            next();
            return std::make_unique<ValueNode>(Value{current_value});
        }
        if (curr()->type == token_types::kw_true) {
            trace("primary true");
            next();
            return std::make_unique<ValueNode>(Value{true});
        }
        if (curr()->type == token_types::kw_false) {
            trace("primary false");
            next();
            return std::make_unique<ValueNode>(Value{false});
        }
        if (curr()->type == token_types::round_bracket_open) {
            trace("primary '('");
            next();
            node_ptr expr = expression();
            if (curr()->type != token_types::round_bracket_close) {
                throw syntax_error{ "unmatched parentheses", curr()->line, curr()->pos};
            }
            next();
            return expr;
        }
        if (curr()->type == token_types::identifier) {
            trace("primary identifier");
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
                trace("###########################################");
                m_current_scope->add_node(statement());
                m_tokens.clear();
                m_index = 0;
            }
        }
        return main_node;
    }
};

} /* namespace mlang */