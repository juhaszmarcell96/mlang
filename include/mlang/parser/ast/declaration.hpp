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
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        env.declare_variable(m_var_name, None::type_name);
        return nullptr;
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
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!rhs) throw RuntimeError{"right hand side of addition returned null"};
        env.declare_variable(m_var_name, None::type_name);
        env.set_variable(m_var_name, rhs);
        return nullptr;
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

class DeclAndInitArrayOperationNode : public Node {
private:
    std::string m_var_name;
    std::vector<node_ptr> m_args;
public:
    DeclAndInitArrayOperationNode(const std::string& var_name) : Node(ast_node_types::declaration), m_var_name(var_name) {}
    ~DeclAndInitArrayOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        std::vector<std::shared_ptr<Object>> args;
        for (const node_ptr& arg : m_args) {
            std::shared_ptr<Object> res = arg->execute(env);
            args.push_back(res);
        }
        env.declare_variable(m_var_name, Array::type_name);
        env.get_variable(m_var_name)->assign(args);
        return nullptr;
    }
    void add_arg (node_ptr arg) {
        m_args.push_back(std::move(arg));
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

} /* namespace mlang */