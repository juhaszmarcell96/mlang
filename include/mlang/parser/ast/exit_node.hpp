#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class ExitNode : public Node {
private:
    node_ptr m_value;
public:
    ExitNode(node_ptr value) : Node(ast_node_types::exit_node), m_value(std::move(value)) {}
    ~ExitNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        std::shared_ptr<Object> val = m_value->execute(env);
        throw Exit { val };
    }
    void print () const override {
        std::cout << "exit ";
        m_value->print();
    }
};

} /* namespace mlang */