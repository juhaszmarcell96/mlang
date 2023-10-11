#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class UnaryNotOperationNode : public Node {
private:
    node_ptr m_right;
public:
    UnaryNotOperationNode(node_ptr right);
    ~UnaryNotOperationNode () = default;
    const Node* const get_right () const;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

class UnaryMinusOperationNode : public Node {
private:
    node_ptr m_right;
public:
    UnaryMinusOperationNode(node_ptr right);
    ~UnaryMinusOperationNode () = default;
    const Node* const get_right () const;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */