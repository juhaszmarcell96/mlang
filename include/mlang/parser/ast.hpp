#pragma once

#include <vector>
#include <iostream>

#include "mlang/value.hpp"
#include "mlang/environment.hpp"
#include "mlang/token.hpp"
#include "mlang/exception.hpp"

namespace mlang {

namespace ast {

} /* namespace ast */

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
    less_equal,
    print
};

class Node;

typedef std::unique_ptr<Node> node_ptr;

class Node {
private:
    ast_node_types m_type { ast_node_types::none };
public:
    Node (ast_node_types type) : m_type(type) {}
    virtual ~Node () = default;
    virtual void execute (Environment& env, Value& return_val) = 0;
    virtual void add_node (node_ptr node) {
        throw unexpected_error{"cannot add nodes this node"};
    }
    virtual Node* get_parent () {
        throw unexpected_error{"this node has no parent"};
        return nullptr;
    }
    virtual void add_elif (node_ptr condition) {
        throw unexpected_error{"unexpected elif"};
    }
    virtual void add_else () {
        throw unexpected_error{"unexpected else"};
    }
    virtual void print () const = 0;

    ast_node_types get_type () const { return m_type; }
};

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
    void print () const override { std::cout << m_value.to_string(); }
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
    void print () const override { std::cout << "var:" << m_var_name; }
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs - rhs;
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs * rhs;
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
    void execute (Environment& env, Value& return_val) override {
        Value lhs {};
        Value rhs {};
        m_left->execute(env, lhs);
        m_right->execute(env, rhs);
        return_val = lhs / rhs;
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        std::cout << " / ";
        m_right->print();
        std::cout << " )";
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
    void print () const override {
        std::cout << m_var_name << "=";
        m_right->print();
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
    void print () const override {
        std::cout << "declare:" << m_var_name;
    }
};

class DeclAndInitOperationNode : public Node {
private:
    value_types m_var_type;
    std::string m_var_name;
    node_ptr m_right;
public:
    DeclAndInitOperationNode(value_types var_type, const std::string& var_name, node_ptr right) : Node(ast_node_types::declaration), m_var_type(var_type), m_var_name(var_name), m_right(std::move(right)) {}
    ~DeclAndInitOperationNode () = default;
    value_types get_var_type () const { return m_var_type; }
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (Environment& env, Value& return_val) override {
        if (env.has_variable(m_var_name)) {
            throw redeclaration_error{m_var_name};
        }
        env.declare_variable(m_var_name, m_var_type);
        Value rhs {};
        m_right->execute(env, rhs);
        Value* m_val = env.get_variable(m_var_name);
        if (m_val->get_type() != m_var_type) {
            throw incompatible_type_error {};
        }
        *m_val = rhs;
    }
    void print () const override {
        std::cout << "declare:" << m_var_name;
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
    void print () const override {
        std::cout << m_var_name << "+=";
        m_right->print();
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
    void print () const override {
        std::cout << m_var_name << "-=";
        m_right->print();
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
    void print () const override {
        std::cout << m_var_name << "*=";
        m_right->print();
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
    void print () const override {
        std::cout << m_var_name << "/=";
        m_right->print();
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
    void print () const override {
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
    }
};

class IfStatementNode : public Node {
private:
    node_ptr m_condition;
    Node* m_parent_scope { nullptr };
    std::vector<node_ptr> m_nodes;

    std::vector<node_ptr> m_elif_conditions;
    std::vector<std::vector<node_ptr>> m_elif_nodes;

    bool m_else_defined { false };
    std::vector<node_ptr> m_else_nodes;

    std::vector<node_ptr>* m_active_branch { nullptr };
public:
    IfStatementNode(node_ptr condition, Node* parent_scope) : Node(ast_node_types::if_statement), m_condition(std::move(condition)), m_parent_scope(parent_scope) {
        m_active_branch = &m_nodes;
    }
    ~IfStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    const Node* const get_condition () const { return m_condition.get(); }
    void execute (Environment& env, Value& return_val) override {
        Value cond_val {};

        /* if */
        m_condition->execute(env, cond_val);
        if (cond_val) {
            for (auto& node : m_nodes) {
                node->execute(env, return_val);
            }
            return;
        }
        /* elif */
        for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
            m_elif_conditions[i]->execute(env, cond_val);
            if (cond_val) {
                for (auto& node : m_elif_nodes[i]) {
                    node->execute(env, return_val);
                }
                return;
            }
        }
        /* else */
        if (m_else_defined) {
            for (auto& node : m_else_nodes) {
                node->execute(env, return_val);
            }
            return;
        }
    }
    void add_node (node_ptr node) override {
        m_active_branch->push_back(std::move(node));
    }
    Node* get_parent () override {
        return m_parent_scope;
    }
    void add_elif (node_ptr condition) override {
        m_elif_conditions.push_back(std::move(condition));
        m_elif_nodes.push_back(std::vector<node_ptr>{});
        m_active_branch = &(m_elif_nodes.back());
    }
    void add_else () override {
        m_else_defined = true;
        m_active_branch = &m_else_nodes;
    }
    void print () const override {
        /* if */
        std::cout << "if ( ";
        m_condition->print();
        std::cout << " )" << std::endl;
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
        /* elif */
        for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
            std::cout << "elif ( ";
            m_elif_conditions[i]->print();
            std::cout << " )" << std::endl;
            for (auto& node : m_elif_nodes[i]) {
                node->print();
                std::cout << std::endl;
            }
        }
        /* else */
        if (m_else_defined) {
            std::cout << "else" << std::endl;
            for (auto& node : m_else_nodes) {
                node->print();
                std::cout << std::endl;
            }
        }
    }
};

class EndStatementNode : public Node {
public:
    EndStatementNode() : Node(ast_node_types::endif_statement) {}
    ~EndStatementNode () = default;
    void execute (Environment& env, Value& return_val) override { }
    void print () const override {
        std::cout << "end";
    }
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
    void execute (Environment& env, Value& return_val) override {
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
    void execute (Environment& env, Value& return_val) override {
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
    void execute (Environment& env, Value& return_val) override {
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
    void execute (Environment& env, Value& return_val) override {
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
    void execute (Environment& env, Value& return_val) override {
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
    void execute (Environment& env, Value& return_val) override {
        Value rhs {};
        m_right->execute(env, rhs);
        return_val = Value{ -rhs };
    }
    void print () const override {
        std::cout << "-";
        m_right->print();
    }
};

class PrintNode : public Node {
private:
    std::string m_rule;
    std::vector<node_ptr> m_args;
    std::size_t m_index { 0 };

    std::string get_integer (Environment& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        Value res {};
        m_args[m_index]->execute(env, res);
        ++m_index;
        return std::to_string(static_cast<int>(res.get_number()));
    }

    std::string get_string (Environment& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        Value res {};
        m_args[m_index]->execute(env, res);
        ++m_index;
        return res.get_string();
    }

    std::string get_float (Environment& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        Value res {};
        m_args[m_index]->execute(env, res);
        ++m_index;
        return std::to_string(res.get_number());
    }

    std::string get_bool (Environment& env) {
        if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
        Value res {};
        m_args[m_index]->execute(env, res);
        ++m_index;
        return (res.get_boolean() ? "true" : "false");
    }
public:
    PrintNode(const std::string& m_rule, std::vector<node_ptr> args) : Node(ast_node_types::print), m_rule(m_rule), m_args(std::move(args)) {}
    ~PrintNode () = default;
    void execute (Environment& env, Value& return_val) override {
        m_index = 0;
        std::string result;
        for  (std::size_t i = 0; i < m_rule.length(); ++i) {
            if (i == m_rule.length() - 1) {
                result.push_back(m_rule[i]);
                break;
            }
            if (m_rule[i] == '%') {
                switch (m_rule[i + 1]) {
                    case 'd' : {
                        result += get_integer(env);
                        ++i;
                        break;
                    }
                    case 'f' : {
                        result += get_float(env);
                        ++i;
                        break;
                    }
                    case 'b' : {
                        result += get_bool(env);
                        ++i;
                        break;
                    }
                    case 's' : {
                        result += get_string(env);
                        ++i;
                        break;
                    }
                    case '%' : {
                        result.push_back(m_rule[i]);
                        ++i;
                        break;
                    }
                    default : {
                        result.push_back(m_rule[i]);
                        break;
                    }
                }
            }
            else {
                result.push_back(m_rule[i]);
            }
        }
        std::cout << result << std::endl;
    }
    void print () const override {
        std::cout << "print(" + m_rule;
        for (const node_ptr& arg : m_args) {
            std::cout << ", ";
            arg->print();
        }
        std::cout << ")";
    }
};

} /* namespace mlang */