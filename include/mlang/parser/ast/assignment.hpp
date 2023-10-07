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
        env.set_variable(m_var_name, rhs.get());
        //return_val = env.get_variable(m_var_name);
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
        if (!rhs) throw RuntimeError{"right hand side of " + operators::add_equal + "' returned null"};
        env.get_variable(m_var_name)->call(operators::add_equal, std::vector<Object*>{ rhs.get() }, return_val);
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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> rhs;
        m_right->execute(env, rhs);
        if (!rhs) throw RuntimeError{"right hand side of " + operators::sub_equal + "' returned null"};
        env.get_variable(m_var_name)->call(operators::sub_equal, std::vector<Object*>{ rhs.get() }, return_val);
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
        if (!rhs) throw RuntimeError{"right hand side of " + operators::mul_equal + "' returned null"};
        env.get_variable(m_var_name)->call(operators::mul_equal, std::vector<Object*>{ rhs.get() }, return_val);
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
        if (!rhs) throw RuntimeError{"right hand side of " + operators::div_equal + "' returned null"};
        env.get_variable(m_var_name)->call(operators::div_equal, std::vector<Object*>{ rhs.get() }, return_val);
    }
    void print () const override {
        std::cout << m_var_name << "/=";
        m_right->print();
    }
};

} /* namespace mlang */