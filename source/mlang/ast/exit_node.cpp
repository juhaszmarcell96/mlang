#include "mlang/ast/exit_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

ExitNode::ExitNode(node_ptr value) : Node(ast_node_types::exit_node), m_value(std::move(value)) {}

object::Object ExitNode::execute (EnvStack& env) const {
    throw Exit { m_value->execute(env) };
}

void ExitNode::print () const {
    std::cout << "exit ";
    m_value->print();
}

} /* namespace ast */
} /* namespace mlang */