#include "mlang/ast/member_function.hpp"

namespace mlang {
namespace ast {

MemberFunctionNode::MemberFunctionNode(node_ptr lhs, const std::string& func_name) : Node(ast_node_types::member_func), m_lhs(std::move(lhs)), m_func_name(func_name) {}

const std::vector<node_ptr>& MemberFunctionNode::get_params () const { return m_params; }

object::Object MemberFunctionNode::execute (script::EnvStack& env) const {
    std::vector<object::Object> params;
    for (const node_ptr& node : m_params) {
        params.push_back(node->execute(env));
    }
    object::Object lhs = m_lhs->execute(env);
    return lhs.call(m_func_name, params);
}

void MemberFunctionNode::add_parameter (node_ptr param) { m_params.push_back(std::move(param)); }

void MemberFunctionNode::print () const {
    m_lhs->print();
    std::cout << "." << m_func_name << " ( ";
    for (std::size_t i = 0; i < m_params.size(); ++i) {
        m_params[i]->print();
        if (i < m_params.size() - 1) { std::cout << ", "; }
    }
    std::cout << " );" << std::endl;
}

} /* namespace ast */
} /* namespace mlang */