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
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        return env.get_variable(m_var_name);
    }
    void print () const override { std::cout << "var:" << m_var_name; }
    
    bool is_lvalue () const override {
        return true;
    }
};

} /* namespace mlang */