#include "mlang/ast/subscript_node.hpp"
#include "mlang/exception.hpp"

namespace mlang {
namespace ast {

SubscriptNode::SubscriptNode(node_ptr lhs) : Node(ast_node_types::subscript), m_lhs(std::move(lhs)) {}

void SubscriptNode::set_index (node_ptr index) { m_index = std::move(index); }

object::Object SubscriptNode::execute (script::EnvStack& env) const {
    object::Object lhs = m_lhs->execute(env);
    object::Object index = m_index->execute(env);
    return lhs.operator_subscript(index);
}

void SubscriptNode::print () const {
    m_lhs->print();
    std::cout << " [ ";
    m_index->print();
    std::cout << " ] ";
}

} /* namespace ast */
} /* namespace mlang */