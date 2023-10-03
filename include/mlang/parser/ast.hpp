#pragma once

#include <vector>
#include <iostream>

#include "mlang/value.hpp"
#include "mlang/environment.hpp"
#include "mlang/token.hpp"
#include "mlang/exception.hpp"

namespace mlang {

enum class ast_node_types {
    none,
    main,
    value,
    variable,
    unary_not,
    unary_minus,
    binary_add,
    binary_sub,
    binary_mul,
    binary_div,
    assignment,
    declaration,
    add_equal,
    sub_equal,
    div_equal,
    mul_equal,
    if_statement,
    endif_statement,
    equality,
    inequality,
    greater,
    less,
    greater_equal,
    less_equal
};

class Node {
private:
    ast_node_types m_type { ast_node_types::none };
public:
    Node (ast_node_types type) : m_type(type) {}
    virtual ~Node () = default;
    virtual void execute (Environment& env, Value& return_val) = 0;
    virtual void add_node (std::unique_ptr<Node> node) {
        throw unexpected_error{"cannot add nodes this node"};
    }
    virtual Node* get_parent () {
        throw unexpected_error{"this node has no parent"};
        return nullptr;
    }

    ast_node_types get_type () const { return m_type; }
};

typedef std::unique_ptr<Node> node_ptr;

class ValueNode : public Node {
private:
    Value m_value;
public:
    explicit ValueNode(Value value) : Node(ast_node_types::value), m_value(value) {}
    ~ValueNode () = default;
    const Value& get_value () const { return m_value; }
    void execute (Environment& env, Value& return_val) override {
        return_val = m_value;
    }
};

class VariableNode : public Node {
private:
    std::string m_var_name;
public:
    explicit VariableNode(const std::string& var_name) : Node(ast_node_types::variable), m_var_name(var_name) {}
    ~VariableNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    void execute (Environment& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value* m_val = env.get_variable(m_var_name);
        return_val = *m_val;
    }
};

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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs + rhs;
    }
};

class BinarySubOperationNode : public BinaryOperationNodeBase {
public:
    BinarySubOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_sub, std::move(left), std::move(right)) {}
    ~BinarySubOperationNode () = default;
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs - rhs;
    }
};

class BinaryMulOperationNode : public BinaryOperationNodeBase {
public:
    BinaryMulOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_mul, std::move(left), std::move(right)) {}
    ~BinaryMulOperationNode () = default;
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs * rhs;
    }
};

class BinaryDivOperationNode : public BinaryOperationNodeBase {
public:
    BinaryDivOperationNode(node_ptr left, node_ptr right) : BinaryOperationNodeBase(ast_node_types::binary_div, std::move(left), std::move(right)) {}
    ~BinaryDivOperationNode () = default;
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs / rhs;
    }
};

class AssignmentOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    AssignmentOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::assignment), m_var_name(var_name), m_right(std::move(right)) {}
    ~AssignmentOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value ret_val {};
        m_right->execute(env, ret_val);
        Value* m_val = env.get_variable(m_var_name);
        if (m_val->get_type() != ret_val.get_type()) {
            throw incompatible_type_error {};
        }
        *m_val = ret_val;
        return_val = *m_val;
    }
};

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
};

class AddEqualOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    AddEqualOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::add_equal), m_var_name(var_name), m_right(std::move(right)) {}
    ~AddEqualOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value rhs {};
        m_right->execute(env, rhs);
        Value* m_val = env.get_variable(m_var_name);
        *m_val += rhs;
        return_val = *m_val;
    }
};

class SubEqualOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    SubEqualOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::sub_equal), m_var_name(var_name), m_right(std::move(right)) {}
    ~SubEqualOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value rhs {};
        m_right->execute(env, rhs);
        Value* m_val = env.get_variable(m_var_name);
        *m_val -= rhs;
        return_val = *m_val;
    }
};

class MulEqualOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    MulEqualOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::mul_equal), m_var_name(var_name), m_right(std::move(right)) {}
    ~MulEqualOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value rhs {};
        m_right->execute(env, rhs);
        Value* m_val = env.get_variable(m_var_name);
        *m_val *= rhs;
        return_val = *m_val;
    }
};

class DivEqualOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    DivEqualOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::div_equal), m_var_name(var_name), m_right(std::move(right)) {}
    ~DivEqualOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value rhs {};
        m_right->execute(env, rhs);
        Value* m_val = env.get_variable(m_var_name);
        *m_val /= rhs;
        return_val = *m_val;
    }
};

class MainNode : public Node {
private:
    std::vector<node_ptr> m_nodes;
public:
    MainNode() : Node(ast_node_types::main) {}
    ~MainNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    void execute (Environment& env, Value& return_val) override {
        for (auto& node : m_nodes) {
            node->execute(env, return_val);
        }
    }
    void add_node (node_ptr node) override {
        m_nodes.push_back(std::move(node));
    }
    Node* get_parent () override {
        throw unexpected_error{"a node of type 'MainNode' has no parent"};
        return nullptr;
    }
};

class IfStatementNode : public Node {
private:
    node_ptr m_condition;
    Node* m_parent_scope { nullptr };
    std::vector<node_ptr> m_nodes;
public:
    IfStatementNode(node_ptr condition, Node* parent_scope) : Node(ast_node_types::if_statement), m_condition(std::move(condition)), m_parent_scope(parent_scope) {}
    ~IfStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    const Node* const get_condition () const { return m_condition.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value cond_val {};
        m_condition->execute(env, cond_val);
        if (cond_val) {
            for (auto& node : m_nodes) {
                node->execute(env, return_val);
            }
        }
    }
    void add_node (node_ptr node) override {
        m_nodes.push_back(std::move(node));
    }
    Node* get_parent () override {
        return m_parent_scope;
    }
};

class EndStatementNode : public Node {
public:
    EndStatementNode() : Node(ast_node_types::endif_statement) {}
    ~EndStatementNode () = default;
    void execute (Environment& env, Value& return_val) override { }
};

class BinaryEqualityOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryEqualityOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::equality), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinaryEqualityOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs == rhs };
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs == rhs };
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs > rhs };
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs < rhs };
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs >= rhs };
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = Value{ lhs <= rhs };
    }
};

class UnaryNotOperationNode : public Node {
private:
    node_ptr m_right;
public:
    UnaryNotOperationNode(node_ptr right) : Node(ast_node_types::unary_not), m_right(std::move(right)) {}
    ~UnaryNotOperationNode () = default;
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value rhs {};
        m_right->execute(env, rhs);
        return_val = Value{ !rhs };
    }
};

class UnaryMinusOperationNode : public Node {
private:
    node_ptr m_right;
public:
    UnaryMinusOperationNode(node_ptr right) : Node(ast_node_types::unary_minus), m_right(std::move(right)) {}
    ~UnaryMinusOperationNode () = default;
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value rhs {};
        m_right->execute(env, rhs);
        return_val = Value{ -rhs };
    }
};

} /* namespace mlang */