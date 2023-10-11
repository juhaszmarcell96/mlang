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

} /* namespace ast */
} /* namespace mlang */