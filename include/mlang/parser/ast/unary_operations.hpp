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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of " + operators::unary_not + "' returned null"};
        rhs->call(operators::unary_not, std::vector<std::shared_ptr<Object>>{}, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of " + operators::unary_minus + "' returned null"};
        rhs->call(operators::unary_minus, std::vector<std::shared_ptr<Object>>{}, return_val);
    }
    void print () const override {
        std::cout << "-";
        m_right->print();
    }
};

} /* namespace mlang */