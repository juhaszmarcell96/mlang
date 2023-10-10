#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class ValueNode : public Node {
private:
    Object m_value;
public:
    explicit ValueNode(Object value) : Node(ast_node_types::value), m_value(value) {}
    ~ValueNode () = default;
    const Object& get_value () const { return m_value; }
    Object execute (EnvStack& env) const override {
        return m_value;
    }
    void print () const override { std::cout << m_value->get_string(); }
};

} /* namespace mlang */