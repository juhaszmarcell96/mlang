#include "mlang/ast/return_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

ReturnNode::ReturnNode(node_ptr value) : Node(ast_node_types::return_node), m_value(std::move(value)) {}

object::Object ReturnNode::execute (EnvStack& env) const {
    /* TODO : handle void returns (nullptr) */
    throw Return { m_value->execute(env) };
}

void ReturnNode::print () const {
    std::cout << "return ";
    m_value->print();
}

} /* namespace ast */
} /* namespace mlang */