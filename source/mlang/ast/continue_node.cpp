#include "mlang/ast/continue_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

ContinueNode::ContinueNode() : Node(ast_node_types::continue_node) {}

object::Object ContinueNode::execute (EnvStack& env) const {
    throw Continue {};
}

void ContinueNode::print () const {
    std::cout << "continue";
}

} /* namespace ast */
} /* namespace mlang */