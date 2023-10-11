#include "mlang/ast/variable_node.hpp"

namespace mlang {
namespace ast {

VariableNode::VariableNode(const std::string& var_name) : Node(ast_node_types::variable), m_var_name(var_name) {}

const std::string& VariableNode::get_var_name () const { return m_var_name; }

object::Object VariableNode::execute (EnvStack& env) const {
    return env.get_variable(m_var_name);
}

void VariableNode::print () const { std::cout << "var:" << m_var_name; }

} /* namespace ast */
} /* namespace mlang */