#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/exception.hpp"

namespace mlang {

class SubscriptNode : public Node {
private:
    node_ptr m_index;
    node_ptr m_lhs;
public:
    explicit SubscriptNode(node_ptr lhs) : Node(ast_node_types::subscript), m_var_name(var_name), m_lhs(std::move(lhs)) {}
    ~SubscriptNode () = default;
    void set_index (node_ptr index) { m_index = std::move(index); }
    Object execute (EnvStack& env) const override {
        Object lhs = m_lhs->execute(env);
        Object index = m_index->execute(env);
        return lhs.operator_array_index(index);
    }
    void print () const override {
        m_lhs->print();
        std::cout << " [ ";
        m_index->print();
        std::cout << " ] ";
    }
    
    bool is_lvalue () const override {
        return true;
    }
};

} /* namespace mlang */