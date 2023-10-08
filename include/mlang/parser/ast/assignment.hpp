#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {
    
class AssignmentOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    AssignmentOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::assignment), m_var_name(var_name), m_right(std::move(right)) {}
    ~AssignmentOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of assignment returned null"};
        env.set_variable(m_var_name, rhs);
        //return_val = env.get_variable(m_var_name);
    }
    void print () const override {
        std::cout << m_var_name << "=";
        m_right->print();
    }
};
    
class ArrayAssignmentOperationNode : public Node {
private:
    std::string m_var_name;
    std::vector<node_ptr> m_args;
public:
    ArrayAssignmentOperationNode(const std::string& var_name) : Node(ast_node_types::assignment), m_var_name(var_name) {}
    ~ArrayAssignmentOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::vector<std::shared_ptr<Object>> args;
        for (node_ptr& arg : m_args) {
            std::shared_ptr<Object> res;
            arg->execute(env, res);
            args.push_back(res);
        }
        std::shared_ptr<Object> var = env.create_value(Array::type_name);
        var->assign(args);
        env.set_variable(m_var_name, var);
    }
    void add_arg (node_ptr arg) {
        m_args.push_back(std::move(arg));
    }
    void print () const override {
        std::cout << m_var_name << " = {...}";
    }
};
    
class ElemAssignmentOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
    node_ptr m_index;
public:
    ElemAssignmentOperationNode(const std::string& var_name, node_ptr right, node_ptr index) : Node(ast_node_types::assignment), m_var_name(var_name), m_right(std::move(right)), m_index(std::move(index)) {}
    ~ElemAssignmentOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_right () const { return m_right.get(); }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of assignment returned null"};
        std::shared_ptr<Object> index;
        m_index->execute(env, index);
        if (!rhs) throw RuntimeError{"index of array element assignment returned null"};
        env.get_variable(m_var_name)->operator_array_index(index.get())->assign(rhs);
    }
    void print () const override {
        std::cout << m_var_name << "=";
        m_right->print();
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of '+=' returned null"};
        env.get_variable(m_var_name)->operator_add_equal(rhs.get());
    }
    void print () const override {
        std::cout << m_var_name << "+=";
        m_right->print();
    }
};
    
class ArrayAddEqualOperationNode : public Node {
private:
    std::string m_var_name;
    std::vector<node_ptr> m_args;
public:
    ArrayAddEqualOperationNode(const std::string& var_name) : Node(ast_node_types::add_equal), m_var_name(var_name) {}
    ~ArrayAddEqualOperationNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::vector<std::shared_ptr<Object>> args;
        for (node_ptr& arg : m_args) {
            std::shared_ptr<Object> res;
            arg->execute(env, res);
            args.push_back(res);
        }
        std::shared_ptr<Object> var = env.create_value(Array::type_name);
        var->assign(args);
        env.get_variable(m_var_name)->operator_add_equal(var.get());
    }
    void add_arg (node_ptr arg) {
        m_args.push_back(std::move(arg));
    }
    void print () const override {
        std::cout << m_var_name << " = {...}";
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of '-=' returned null"};
        env.get_variable(m_var_name)->operator_sub_equal(rhs.get());
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of '*=' returned null"};
        env.get_variable(m_var_name)->operator_mul_equal(rhs.get());
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of '/=' returned null"};
        env.get_variable(m_var_name)->operator_div_equal(rhs.get());
    }
    void print () const override {
        std::cout << m_var_name << "/=";
        m_right->print();
    }
};

} /* namespace mlang */