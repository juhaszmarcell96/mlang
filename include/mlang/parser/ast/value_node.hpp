#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class ValueNode : public Node {
private:
    Value m_value;
public:
    explicit ValueNode(Value value) : Node(ast_node_types::value), m_value(value) {}
    ~ValueNode () = default;
    const Value& get_value () const { return m_value; }
    void execute (EnvStack& env, Value& return_val) override {
        return_val = m_value;
    }
    void print () const override { std::cout << m_value.to_string(); }
};

} /* namespace mlang */