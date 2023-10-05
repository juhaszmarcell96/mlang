#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class BinaryEqualityOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryEqualityOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::equality), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinaryEqualityOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs == rhs };
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " == ";
        m_right->print();
        std::cout << " )";
    }
};

class BinaryInequalityOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryInequalityOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::inequality), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinaryInequalityOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs != rhs };
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " != ";
        m_right->print();
        std::cout << " )";
    }
};

class ComparisonGreaterNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    ComparisonGreaterNode(node_ptr left, node_ptr right) : Node(ast_node_types::greater), m_left(std::move(left)), m_right(std::move(right)) {}
    ~ComparisonGreaterNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs > rhs };
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " > ";
        m_right->print();
        std::cout << " )";
    }
};

class ComparisonLessNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    ComparisonLessNode(node_ptr left, node_ptr right) : Node(ast_node_types::less), m_left(std::move(left)), m_right(std::move(right)) {}
    ~ComparisonLessNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs < rhs };
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " < ";
        m_right->print();
        std::cout << " )";
    }
};

class ComparisonGreaterEqualNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    ComparisonGreaterEqualNode(node_ptr left, node_ptr right) : Node(ast_node_types::greater_equal), m_left(std::move(left)), m_right(std::move(right)) {}
    ~ComparisonGreaterEqualNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs >= rhs };
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " >= ";
        m_right->print();
        std::cout << " )";
    }
};

class ComparisonLessEqualNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    ComparisonLessEqualNode(node_ptr left, node_ptr right) : Node(ast_node_types::less_equal), m_left(std::move(left)), m_right(std::move(right)) {}
    ~ComparisonLessEqualNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs <= rhs };
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " <= ";
        m_right->print();
        std::cout << " )";
    }
};

} /* namespace mlang */