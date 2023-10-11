#include "mlang/ast/unary_operations.hpp"

namespace mlang {
namespace ast {

UnaryNotOperationNode::UnaryNotOperationNode(node_ptr right) : Node(ast_node_types::unary_not), m_right(std::move(right)) {}

const Node* const UnaryNotOperationNode::get_right () const { return m_right.get(); }

object::Object UnaryNotOperationNode::execute (EnvStack& env) const {
    return m_right->execute(env).unary_not();
}

void UnaryNotOperationNode::print () const {
    std::cout << "!";
    m_right->print();
}




UnaryMinusOperationNode::UnaryMinusOperationNode(node_ptr right) : Node(ast_node_types::unary_minus), m_right(std::move(right)) {}

const Node* const UnaryMinusOperationNode::get_right () const { return m_right.get(); }

object::Object UnaryMinusOperationNode::execute (EnvStack& env) const {
    return m_right->execute(env).unary_minus();
}

void UnaryMinusOperationNode::print () const {
    std::cout << "-";
    m_right->print();
}






PostfixIncrementNode::PostfixIncrementNode(node_ptr exp) : Node(ast_node_types::postfix), m_exp(std::move(exp)) {}

object::Object PostfixIncrementNode::execute (EnvStack& env) const {
    return m_exp->execute(env).postfix_increment();
}

void PostfixIncrementNode::print () const {
    m_exp->print();
    std::cout << "++";
}




PostfixDecrementNode::PostfixDecrementNode(node_ptr exp) : Node(ast_node_types::postfix), m_exp(std::move(exp)) {}

object::Object PostfixDecrementNode::execute (EnvStack& env) const {
    return m_exp->execute(env).postfix_decrement();
}

void PostfixDecrementNode::print () const {
    m_exp->print();
    std::cout << "--";
}




PrefixIncrementNode::PrefixIncrementNode(node_ptr exp) : Node(ast_node_types::prefix), m_exp(std::move(exp)) {}

object::Object PrefixIncrementNode::execute (EnvStack& env) const {
    return m_exp->execute(env).prefix_increment();
}

void PrefixIncrementNode::print () const {
    std::cout << "++";
    m_exp->print();
}




PrefixDecrementNode::PrefixDecrementNode(node_ptr exp) : Node(ast_node_types::prefix), m_exp(std::move(exp)) {}

object::Object PrefixDecrementNode::execute (EnvStack& env) const {
    return m_exp->execute(env).prefix_decrement();
}

void PrefixDecrementNode::print () const {
    std::cout << "--";
    m_exp->print();
}

} /* namespace ast */
} /* namespace mlang */