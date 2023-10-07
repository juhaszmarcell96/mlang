#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/exception.hpp"

namespace mlang {

class IndexingNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_index;
public:
    explicit IndexingNode(const std::string& var_name, node_ptr index) : Node(ast_node_types::indexing_node), m_var_name(var_name), m_index(std::move(index)) {}
    ~IndexingNode () = default;
    const std::string& get_var_name () const { return m_var_name; }
    const Node* const get_index () const { return m_index.get(); }
    void execute (EnvStack& env, Value& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        Value index {};
        m_index->execute(env, index);
        if (index.get_type() != value_types::number) {
            throw RuntimeError { "invalid array indexing" };
        }
        return_val = (*(env.get_variable(m_var_name)))[static_cast<std::size_t>(index.get_number())];
    }
    void print () const override { std::cout << "var:" << m_var_name; }
    
    bool is_lvalue () const override {
        return true;
    }
};

} /* namespace mlang */