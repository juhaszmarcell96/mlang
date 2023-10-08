#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class PrintNode : public Node {
private:
    std::string m_rule;
    std::vector<node_ptr> m_args;
    std::size_t m_index { 0 };

    std::string get_integer (EnvStack& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        std::shared_ptr<Object> res = m_args[m_index]->execute(env);
        if (!res) { throw RuntimeError { "print expression returned null" }; }
        ++m_index;
        return std::to_string(static_cast<int>(res->get_number()));
    }

    std::string get_string (EnvStack& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        std::shared_ptr<Object> res = m_args[m_index]->execute(env);
        if (!res) { throw RuntimeError { "print expression returned null" }; }
        ++m_index;
        return res->get_string();
    }

    std::string get_float (EnvStack& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        std::shared_ptr<Object> res = m_args[m_index]->execute(env);
        if (!res) { throw RuntimeError { "print expression returned null" }; }
        ++m_index;
        return std::to_string(res->get_number());
    }

    std::string get_bool (EnvStack& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        std::shared_ptr<Object> res = m_args[m_index]->execute(env);
        if (!res) { throw RuntimeError { "print expression returned null" }; }
        ++m_index;
        return (res->is_true() ? "true" : "false");
    }
public:
    PrintNode(const std::string& m_rule, std::vector<node_ptr> args) : Node(ast_node_types::print), m_rule(m_rule), m_args(std::move(args)) {}
    ~PrintNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) override {
        m_index = 0;
        std::string result;
        for  (std::size_t i = 0; i < m_rule.length(); ++i) {
            if (i == m_rule.length() - 1) {
                result.push_back(m_rule[i]);
                break;
            }
            if (m_rule[i] == '%') {
                switch (m_rule[i + 1]) {
                    case 'd' : {
                        result += get_integer(env);
                        ++i;
                        break;
                    }
                    case 'f' : {
                        result += get_float(env);
                        ++i;
                        break;
                    }
                    case 'b' : {
                        result += get_bool(env);
                        ++i;
                        break;
                    }
                    case 's' : {
                        result += get_string(env);
                        ++i;
                        break;
                    }
                    case '%' : {
                        result.push_back(m_rule[i]);
                        ++i;
                        break;
                    }
                    default : {
                        result.push_back(m_rule[i]);
                        break;
                    }
                }
            }
            else {
                result.push_back(m_rule[i]);
            }
        }
        std::cout << result << std::endl;
        return nullptr;
    }
    void print () const override {
        std::cout << "print(" + m_rule;
        for (const node_ptr& arg : m_args) {
            std::cout << ", ";
            arg->print();
        }
        std::cout << ")";
    }
};

} /* namespace mlang */