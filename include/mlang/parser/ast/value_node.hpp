#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class ValueNode : public Node {
private:
    std::shared_ptr<Object> m_value;
public:
    explicit ValueNode(std::shared_ptr<Object> value) : Node(ast_node_types::value), m_value(value) {}
    ~ValueNode () = default;
    const std::shared_ptr<Object> get_value () const { return m_value; }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        return_val = m_value;
    }
    void print () const override { std::cout << m_value->get_string(); }
};

} /* namespace mlang */