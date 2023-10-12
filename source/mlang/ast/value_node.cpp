#include "mlang/ast/value_node.hpp"

namespace mlang {
namespace ast {

ValueNode::ValueNode(object::Object value) : Node(ast_node_types::value), m_value(value) {}

const object::Object& ValueNode::get_value () const { return m_value; }

object::Object ValueNode::execute (script::EnvStack& env) const {
    return m_value;
}

void ValueNode::print () const { std::cout << m_value.get_string(); }

} /* namespace ast */
} /* namespace mlang */