#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class EndStatementNode : public Node {
public:
    EndStatementNode() : Node(ast_node_types::end_statement) {}
    ~EndStatementNode () = default;
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {}
    void print () const override {
        std::cout << "end";
    }
};

} /* namespace mlang */