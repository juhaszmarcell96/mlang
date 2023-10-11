#include "mlang/ast/block_node.hpp"

namespace mlang {
namespace ast {

BlockNode::BlockNode() : Node(ast_node_types::main) {}

const std::vector<node_ptr>& BlockNode::get_nodes () const { return m_nodes; }

object::Object BlockNode::execute (EnvStack& env) const {
    for (const auto& node : m_nodes) {
        node->execute(env);
    }
    return object::Object{};
}

void BlockNode::add_node (node_ptr node) {
    m_nodes.push_back(std::move(node));
}

void BlockNode::print () const {
    for (const auto& node : m_nodes) {
        node->print();
        std::cout << std::endl;
    }
}

} /* namespace ast */
} /* namespace mlang */