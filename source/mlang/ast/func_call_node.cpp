#include "mlang/ast/func_call_node.hpp"
#include "mlang/ast/func_decl_node.hpp"

namespace mlang {
namespace ast {
    
FunctionCallNode::FunctionCallNode(const std::string& name) : Node(ast_node_types::func_call), m_name(name) {}

const std::vector<node_ptr>& FunctionCallNode::get_params () const { return m_params; }

object::Object FunctionCallNode::execute (script::EnvStack& env) const {
    std::vector<object::Object> params;
    for (const node_ptr& node : m_params) {
        params.push_back(node->execute(env));
    }
    return env.get_function(m_name)->call(env, params);
}

void FunctionCallNode::add_parameter (node_ptr param) {
    m_params.push_back(std::move(param));
}

void FunctionCallNode::print () const {
    std::cout << m_name << " ( ";
    for (std::size_t i = 0; i < m_params.size(); ++i) {
        m_params[i]->print();
        if (i < m_params.size() - 1) { std::cout << ", "; }
    }
    std::cout << " );" << std::endl;
}

} /* namespace ast */
} /* namespace mlang */