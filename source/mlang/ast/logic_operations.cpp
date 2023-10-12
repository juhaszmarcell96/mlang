#include "mlang/ast/logic_operations.hpp"

namespace mlang {
namespace ast {

BinaryLogicNode::BinaryLogicNode(node_ptr left, node_ptr right, logic_mode mode) : Node(ast_node_types::logic),
                                                                                   m_left(std::move(left)),
                                                                                   m_right(std::move(right)),
                                                                                   m_mode(mode) {}

const Node* const BinaryLogicNode::get_left () const { return m_left.get(); }

const Node* const BinaryLogicNode::get_right () const { return m_right.get(); }

logic_mode BinaryLogicNode::get_mode () const { return m_mode; }

object::Object BinaryLogicNode::execute (script::EnvStack& env) const {
    object::Object lhs = m_left->execute(env);
    object::Object rhs = m_right->execute(env);
    switch (m_mode) {
        case logic_mode::logic_and : { return lhs.operator_binary_and(rhs); }
        case logic_mode::logic_or  : { return lhs.operator_binary_or(rhs); }
        default : { break; }
    }
    throw RuntimeError{"invalid logic operator type"};
}

void BinaryLogicNode::print () const {
    std::cout << "( ";
    m_left->print();
    switch (m_mode) {
        case logic_mode::logic_and : { std::cout << " && "; break; }
        case logic_mode::logic_or  : { std::cout << " || "; break; }
        default : { std::cout << " ?? "; break; }
    }
    m_right->print();
    std::cout << " )";
}

} /* namespace ast */
} /* namespace mlang */