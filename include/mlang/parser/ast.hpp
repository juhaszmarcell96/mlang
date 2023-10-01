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
    value,
    variable,
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
    if_statement
};

class Node {
private:
    ast_node_types m_type { ast_node_types::none };
public:
    Node (ast_node_types type) : m_type(type) {}
    virtual ~Node () = default;
    virtual void execute (Environment& env, Value& return_val) = 0;

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

class BinaryAddOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryAddOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::binary_add), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinaryAddOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs + rhs;
    }
};

class BinarySubOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinarySubOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::binary_sub), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinarySubOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs - rhs;
    }
};

class BinaryMulOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryMulOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::binary_mul), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinaryMulOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs * rhs;
    }
};

class BinaryDivOperationNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
public:
    BinaryDivOperationNode(node_ptr left, node_ptr right) : Node(ast_node_types::binary_div), m_left(std::move(left)), m_right(std::move(right)) {}
    ~BinaryDivOperationNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
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

/*
class IfNode : public Node {
private:
    node_ptr m_condition;
    std::size_t m_pc;
public:
    IfNode(node_ptr right, std::size_t program_counter) : Node(ast_node_types::if_statement), m_var_name(var_name), m_right(std::move(right)) {}
    ~IfNode () = default;
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
*/

} /* namespace mlang */