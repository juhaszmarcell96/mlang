#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class UnaryNotOperationNode : public Node {
private:
    node_ptr m_right;
public:
    UnaryNotOperationNode(node_ptr right) : Node(ast_node_types::unary_not), m_right(std::move(right)) {}
    ~UnaryNotOperationNode () = default;
    const Node* const get_right () const { return m_right.get(); }
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!rhs) throw RuntimeError{"right hand side of 'unary !' returned null"};
        return rhs->unary_not();
    }
    void print () const override {
        std::cout << "!";
        m_right->print();
    }
};

class UnaryMinusOperationNode : public Node {
private:
    node_ptr m_right;
public:
    UnaryMinusOperationNode(node_ptr right) : Node(ast_node_types::unary_minus), m_right(std::move(right)) {}
    ~UnaryMinusOperationNode () = default;
    const Node* const get_right () const { return m_right.get(); }
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!rhs) throw RuntimeError{"right hand side of 'unary -' returned null"};
        return rhs->unary_minus();
    }
    void print () const override {
        std::cout << "-";
        m_right->print();
    }
};

} /* namespace mlang */