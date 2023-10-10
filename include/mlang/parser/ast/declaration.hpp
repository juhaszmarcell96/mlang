#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/array.hpp"

namespace mlang {

class DeclarationOperationNode : public Node {
private:
    std::string m_var_name;
public:
    DeclarationOperationNode(const std::string& var_name) : Node(ast_node_types::declaration), m_var_name(var_name) {}
    ~DeclarationOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    Object execute (EnvStack& env) const override {
        env.declare_variable(m_var_name, None::type_name);
        return Object{};
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

class DeclAndInitOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    DeclAndInitOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::declaration), m_var_name(var_name), m_right(std::move(right)) {}
    ~DeclAndInitOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    Object execute (EnvStack& env) const override {
        Object rhs = m_right->execute(env);
        env.declare_variable(m_var_name, None::type_name);
        env.set_variable(m_var_name, rhs);
        return Object{};
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

} /* namespace mlang */