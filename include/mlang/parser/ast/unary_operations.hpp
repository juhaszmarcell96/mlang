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
    void execute (EnvStack& env, Value& return_val) override {
        Value rhs {};
        m_right->execute(env, rhs);
        return_val = Value{ !rhs };
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
    void execute (EnvStack& env, Value& return_val) override {
        Value rhs {};
        m_right->execute(env, rhs);
        return_val = Value{ -rhs };
    }
    void print () const override {
        std::cout << "-";
        m_right->print();
    }
};

} /* namespace mlang */