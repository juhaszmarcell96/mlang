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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::shared_ptr<Object> val;
        m_value->execute(env, val);
        throw Return { val };
    }
    void print () const override {
        std::cout << "return ";
        m_value->print();
    }
};

} /* namespace mlang */