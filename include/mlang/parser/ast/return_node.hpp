#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class ReturnNode : public Node {
private:
    node_ptr m_value;
public:
    ReturnNode(node_ptr value) : Node(ast_node_types::return_node), m_value(std::move(value)) {}
    ~ReturnNode () = default;
    Object execute (EnvStack& env) const override {
        /* TODO : handle void returns (nullptr) */
        throw Return { m_value->execute(env) };
    }
    void print () const override {
        std::cout << "return ";
        m_value->print();
    }
};

} /* namespace mlang */