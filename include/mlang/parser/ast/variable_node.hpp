#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class VariableNode : public Node {
private:
    std::string m_var_name;
public:
    explicit VariableNode(const std::string& var_name) : Node(ast_node_types::variable), m_var_name(var_name) {}
    ~VariableNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    void execute (EnvStack& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value* m_val = env.get_variable(m_var_name);
        return_val = *m_val;
    }
    void print () const override { std::cout << "var:" << m_var_name; }
    
    bool is_lvalue () const override {
        return true;
    }
};

} /* namespace mlang */