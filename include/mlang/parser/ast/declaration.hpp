#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class DeclarationOperationNode : public Node {
private:
    value_types m_var_type;
    std::string m_var_name;
public:
    DeclarationOperationNode(value_types var_type, const std::string& var_name) : Node(ast_node_types::declaration), m_var_type(var_type), m_var_name(var_name) {}
    ~DeclarationOperationNode () = default;
    value_types get_var_type () const { return m_var_type; }
    const std::string& get_var_name () const { return m_var_name; }
    void execute (Environment& env, Value& return_val) override {
        if (env.has_variable(m_var_name)) {
            throw redeclaration_error{m_var_name};
        }
        env.declare_variable(m_var_name, m_var_type);
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

class DeclAndInitOperationNode : public Node {
private:
    value_types m_var_type;
    std::string m_var_name;
    node_ptr m_right;
public:
    DeclAndInitOperationNode(value_types var_type, const std::string& var_name, node_ptr right) : Node(ast_node_types::declaration), m_var_type(var_type), m_var_name(var_name), m_right(std::move(right)) {}
    ~DeclAndInitOperationNode () = default;
    value_types get_var_type () const { return m_var_type; }
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (env.has_variable(m_var_name)) {
            throw redeclaration_error{m_var_name};
        }
        env.declare_variable(m_var_name, m_var_type);
        Value rhs {};
        m_right->execute(env, rhs);
        Value* m_val = env.get_variable(m_var_name);
        if (m_val->get_type() != m_var_type) {
            throw incompatible_type_error {};
        }
        *m_val = rhs;
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

} /* namespace mlang */