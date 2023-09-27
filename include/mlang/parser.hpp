#pragma once

#include <vector>

#include "token.hpp"
#include "environment.hpp"
#include "value.hpp"
#include "ast.hpp"

namespace lang {

typedef std::vector<Token*> tokens_vec;

class Parser {
private:
    const tokens_vec& m_tokens;

    std::size_t m_index { 0 };

    void next(int num = 1) { m_index = std::min(m_index + num, m_tokens.size()); }
    bool done() const { return m_index >= m_tokens.size(); }
    const Token* peek (int num = 1) const {
        if ((m_index + num >= 0) && (m_index + num < m_tokens.size())) {
            return m_tokens[m_index + num];
        }
        return nullptr;
    }
    const Token* curr() const { return m_tokens[m_index]; }

    node_ptr expression() {
        /* the highest level parsing function calls the function for the lowest precedence operation */
        token_types current_type = curr()->type;
        switch (current_type) {
            case token_types::number : {
                token_types next_type = peek()->type;
                switch (next_type) {
                    //case token_types::kw_string:
                    //case token_types::kw_number: {
                    //	return declaration();
                    //}
                    case token_types::equal_sign: {
                        return assignment();
                    }
                    default: {
                        return add_sub();
                    }
                }
                break;
            }
            default : {
                return nullptr;
            }
        }
    }

    node_ptr assignment() {
        node_ptr expr = primary();
        if (done()) return expr;
        while (curr()->type == token_types::equal_sign) {
            next();
            node_ptr rhs = add_sub();
            expr = std::make_unique<AssignmentOperationNode>(std::move(expr), std::move(rhs));
            if (done()) return expr;
        }
        return expr;
    }

    node_ptr add_sub() {
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

    node_ptr mul_div() {
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

    node_ptr unary() {
        // for now, let's just return a primary.
        /* TODO : implement ++ and -- */
        return primary();
    }

    node_ptr primary() {
        if (curr()->type == token_types::number) {
            double current_value = curr()->value_num;
            next();
            return std::make_unique<ValueNode>(Value{current_value});
        }
        if (curr()->type == token_types::round_bracket_open) {
            next();
            node_ptr expr = expression();
            if (curr()->type != token_types::round_bracket_open) {
                // error: unmatched parentheses
            }
            next();
            return expr;
        }
        // error: unexpected token
    }
public:
    Parser() = delete;
    explicit Parser (const tokens_vec& tokens) : m_tokens(tokens) {}

    node_ptr parse() {
        m_index = 0;
        return expression();
    }
};

} /* namespace lang */