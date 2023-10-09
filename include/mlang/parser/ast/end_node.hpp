#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class EndStatementNode : public Node {
public:
    EndStatementNode() : Node(ast_node_types::end_statement) {}
    ~EndStatementNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) const override { return nullptr; }
    void print () const override {
        std::cout << "end";
    }
};

} /* namespace mlang */