#include "mlang/ast/declaration.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/array.hpp"

namespace mlang {
namespace ast {

DeclarationOperationNode::DeclarationOperationNode(const std::string& var_name) : Node(ast_node_types::declaration), m_var_name(var_name) {}

const std::string& DeclarationOperationNode::get_var_name () const { return m_var_name; }

object::Object DeclarationOperationNode::execute (EnvStack& env) const {
    env.declare_variable(m_var_name, object::None::type_name);
    return object::Object{};
}

void DeclarationOperationNode::print () const {
    std::cout << "declare:" << m_var_name;
}




DeclAndInitOperationNode::DeclAndInitOperationNode(const std::string& var_name, node_ptr right) : Node(ast_node_types::declaration), m_var_name(var_name), m_right(std::move(right)) {}

const std::string& DeclAndInitOperationNode::get_var_name () const { return m_var_name; }

const Node* const DeclAndInitOperationNode::get_right () const { return m_right.get(); }

object::Object DeclAndInitOperationNode::execute (EnvStack& env) const {
    object::Object rhs = m_right->execute(env);
    env.declare_variable(m_var_name, object::None::type_name);
    env.get_variable(m_var_name).assign(rhs);
    return object::Object{};
}

void DeclAndInitOperationNode::print () const {
    std::cout << "declare:" << m_var_name;
}

} /* namespace ast */
} /* namespace mlang */