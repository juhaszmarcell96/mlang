#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class ContinueNode : public Node {
public:
    ContinueNode() : Node(ast_node_types::continue_node) {}
    ~ContinueNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        throw Continue {};
    }
    void print () const override {
        std::cout << "continue";
    }
};

} /* namespace mlang */