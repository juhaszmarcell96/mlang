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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        if (!env.has_variable(m_var_name)) {
            throw undefined_var_error{m_var_name};
        }
        std::shared_ptr<Object> index;
        m_index->execute(env, index);
        env.get_variable(m_var_name)->call(operators::arr_indexing, std::vector<std::shared_ptr<Object>>{ index }, return_val);
    }
    void print () const override {
        std::cout << m_var_name << "[";
        m_index->print();
        std::cout << "]";
    }
    
    bool is_lvalue () const override {
        return true;
    }
};

} /* namespace mlang */