#include "mlang/ast/member_access.hpp"

namespace mlang {
namespace ast {

MemberAccessNode::MemberAccessNode(node_ptr lhs, const std::string& member_name) : Node(ast_node_types::member_access), m_lhs(std::move(lhs)), m_member_name(member_name) {}

object::Object MemberAccessNode::execute (script::EnvStack& env) const {
    object::Object lhs = m_lhs->execute(env);
    return lhs.access(m_member_name);
}

void MemberAccessNode::print () const {
    m_lhs->print();
    std::cout << "." << m_member_name;
}

} /* namespace ast */
} /* namespace mlang */