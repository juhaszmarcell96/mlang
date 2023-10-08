#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class BinaryOperationNodeBase : public Node {
protected:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryOperationNodeBase(ast_node_types type, node_ptr left, node_ptr right) : Node(type), m_left(std::move(left)), m_right(std::move(right)) {}
    virtual ~BinaryOperationNodeBase () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
};

class BinaryAddOperationNode : public BinaryOperationNodeBase {
public:
    BinaryAddOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_add, std::move(left), std::move(right)) {}
    ~BinaryAddOperationNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) override {
        std::shared_ptr<Object> lhs = m_left->execute(env);
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!lhs) throw RuntimeError{"left hand side of '+' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of '+' returned null"};
        return lhs->operator_binary_add(rhs.get());
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " + ";
        m_right->print();
        std::cout << " )";
    }
};

class BinarySubOperationNode : public BinaryOperationNodeBase {
public:
    BinarySubOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_sub, std::move(left), std::move(right)) {}
    ~BinarySubOperationNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) override {
        std::shared_ptr<Object> lhs = m_left->execute(env);
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!lhs) throw RuntimeError{"left hand side of '-' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of '-' returned null"};
        return lhs->operator_binary_sub(rhs.get());
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " - ";
        m_right->print();
        std::cout << " )";
    }
};

class BinaryMulOperationNode : public BinaryOperationNodeBase {
public:
    BinaryMulOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_mul, std::move(left), std::move(right)) {}
    ~BinaryMulOperationNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) override {
        std::shared_ptr<Object> lhs = m_left->execute(env);
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!lhs) throw RuntimeError{"left hand side of '*' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of '*' returned null"};
        return lhs->operator_binary_mul(rhs.get());
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " * ";
        m_right->print();
        std::cout << " )";
    }
};

class BinaryDivOperationNode : public BinaryOperationNodeBase {
public:
    BinaryDivOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_div, std::move(left), std::move(right)) {}
    ~BinaryDivOperationNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) override {
        std::shared_ptr<Object> lhs = m_left->execute(env);
        std::shared_ptr<Object> rhs = m_right->execute(env);
        if (!lhs) throw RuntimeError{"left hand side of '/' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of '/' returned null"};
        return lhs->operator_binary_div(rhs.get());
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " / ";
        m_right->print();
        std::cout << " )";
    }
};

} /* namespace mlang */