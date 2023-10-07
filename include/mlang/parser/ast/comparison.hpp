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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of " + operators::binary_equality + "' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of " + operators::binary_equality + "' returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call(operators::binary_equality, std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of " + operators::binary_inequality + "' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of " + operators::binary_inequality + "' returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call(operators::binary_inequality, std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of " + operators::comparison_greater + "' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of " + operators::comparison_greater + "' returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call(operators::comparison_greater, std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of " + operators::comparison_less + "' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of " + operators::comparison_less + "' returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call(operators::comparison_less, std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of " + operators::comparison_greater_equal + "' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of " + operators::comparison_greater_equal + "' returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call(operators::comparison_greater_equal, std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> lhs;
        std::shared_ptr<Object> rhs;
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        if (!lhs) throw RuntimeError{"left hand side of " + operators::comparison_less_equal + "' returned null"};
        if (!rhs) throw RuntimeError{"right hand side of " + operators::comparison_less_equal + "' returned null"};
        return_val = env.create_value(lhs->get_typename());
        lhs->call(operators::comparison_less_equal, std::vector<Object*>{ rhs.get() }, return_val);
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