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

class PostfixIncrementNode : public Node {
private:
    node_ptr m_exp;
public:
    PostfixIncrementNode(node_ptr exp);
    ~PostfixIncrementNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

class PostfixDecrementNode : public Node {
private:
    node_ptr m_exp;
public:
    PostfixDecrementNode(node_ptr exp);
    ~PostfixDecrementNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

class PrefixIncrementNode : public Node {
private:
    node_ptr m_exp;
public:
    PrefixIncrementNode(node_ptr exp);
    ~PrefixIncrementNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

class PrefixDecrementNode : public Node {
private:
    node_ptr m_exp;
public:
    PrefixDecrementNode(node_ptr exp);
    ~PrefixDecrementNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */