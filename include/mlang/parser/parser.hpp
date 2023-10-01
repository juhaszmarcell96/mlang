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

    void trace (const std::string& str) const {
        #if TRACE_PARSER == 1
        std::cout << str << std::endl;
        #endif
    }

    node_ptr expression() {
        /* the highest level parsing function calls the function for the lowest precedence operation */
        token_types current_type = curr()->type;
        switch (current_type) {
            case token_types::kw_string:
            case token_types::kw_number:
            case token_types::kw_array:
            case token_types::kw_bool: {
                return declaration();
            }
            case token_types::identifier: {
                token_types next_type = peek()->type;
                switch (next_type) {
                    case token_types::equal_sign: {
                        return assignment();
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

    node_ptr declaration() {
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

    node_ptr assignment() {
        trace("assignment");
        std::string variable_name = curr()->value_str;
        next();
        if (done()) { throw syntax_error{ "assignment too shot", curr()->line, curr()->pos}; }
        if (curr()->type != token_types::equal_sign) { throw syntax_error{ "did not find variable name", curr()->line, curr()->pos}; }
        next();
        node_ptr rhs = expression();
        return std::make_unique<AssignmentOperationNode>(variable_name, std::move(rhs));
    }

    node_ptr add_sub() {
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

    node_ptr mul_div() {
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

    node_ptr unary() {
        trace("unary");
        // for now, let's just return a primary.
        /* TODO : implement ++ and -- */
        return primary();
    }

    node_ptr primary() {
        trace("primary");
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
        if (curr()->type == token_types::identifier) {
            std::string current_str = curr()->value_str;
            next();
            return std::make_unique<VariableNode>(current_str);
        }
        throw syntax_error{ "unexpected primary token", curr()->line, curr()->pos};
        return nullptr;
    }
public:
    Parser() = delete;
    explicit Parser (const tokens_vec& tokens) : m_tokens(tokens) {}

    node_ptr parse() {
        m_index = 0;
        //std::cout << "parsing : " << std::endl;
        //for (const Token* token : m_tokens) {
        //    std::cout << token->get_for_print() << std::endl;
        //}
        return expression();
    }
};

} /* namespace mlang */