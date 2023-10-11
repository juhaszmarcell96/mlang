#include "mlang/ast/main_node.hpp"

namespace mlang {
namespace ast {

MainNode::MainNode() : Node(ast_node_types::main) {}

const std::vector<node_ptr>& MainNode::get_nodes () const { return m_nodes; }

object::Object MainNode::execute (EnvStack& env) const {
    for (auto& node : m_nodes) {
        node->execute(env);
    }
    /* return null TODO */
    return object::Object {};
}

void MainNode::add_node (node_ptr node) {
    m_nodes.push_back(std::move(node));
}

void MainNode::print () const {
    for (auto& node : m_nodes) {
        node->print();
        std::cout << std::endl;
    }
}

} /* namespace ast */
} /* namespace mlang */