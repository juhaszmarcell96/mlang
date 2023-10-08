#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class BreakNode : public Node {
public:
    BreakNode() : Node(ast_node_types::break_node) {}
    ~BreakNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) override {
        throw Break {};
    }
    void print () const override {
        std::cout << "break";
    }
};

} /* namespace mlang */