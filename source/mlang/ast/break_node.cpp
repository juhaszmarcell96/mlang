#include "mlang/ast/break_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

BreakNode::BreakNode() : Node(ast_node_types::break_node) {}

object::Object BreakNode::execute (script::EnvStack& env) const {
    throw Break {};
}

void BreakNode::print () const {
    std::cout << "break";
}

} /* namespace ast */
} /* namespace mlang */