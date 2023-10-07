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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of addition returned null"};
        if (!rhs) throw RuntimeError{"right hand side of addition returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call("binary_add", std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of addition returned null"};
        if (!rhs) throw RuntimeError{"right hand side of addition returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call("binary_sub", std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of addition returned null"};
        if (!rhs) throw RuntimeError{"right hand side of addition returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call("binary_mul", std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of addition returned null"};
        if (!rhs) throw RuntimeError{"right hand side of addition returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call("binary_div", std::vector<Object*>{ rhs.get() }, return_val);
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