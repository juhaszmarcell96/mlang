#include "mlang/ast/binary_operations.hpp"

namespace mlang {
namespace ast {

BinaryArithmeticNode::BinaryArithmeticNode(node_ptr left, node_ptr right, arithmetic_mode mode) : Node(ast_node_types::binary_arith),
                                                                                                  m_left(std::move(left)),
                                                                                                  m_right(std::move(right)),
                                                                                                  m_mode(mode) {}

const Node* const BinaryArithmeticNode::get_left () const { return m_left.get(); }

const Node* const BinaryArithmeticNode::get_right () const { return m_right.get(); }

arithmetic_mode BinaryArithmeticNode::get_mode () const { return m_mode; }

object::Object BinaryArithmeticNode::execute (EnvStack& env) const {
    object::Object lhs = m_left->execute(env);
    object::Object rhs = m_right->execute(env);
    switch (m_mode) {
        case arithmetic_mode::add : { return lhs.operator_binary_add(rhs); }
        case arithmetic_mode::sub : { return lhs.operator_binary_sub(rhs); }
        case arithmetic_mode::mul : { return lhs.operator_binary_mul(rhs); }
        case arithmetic_mode::div : { return lhs.operator_binary_div(rhs); }
        default : { break; }
    }
    throw RuntimeError{"invalid arithmetic operator type"};
}

void BinaryArithmeticNode::print () const {
    std::cout << "( ";
    m_left->print();
    switch (m_mode) {
        case arithmetic_mode::add : { std::cout << " + "; break; }
        case arithmetic_mode::sub : { std::cout << " - "; break; }
        case arithmetic_mode::mul : { std::cout << " * "; break; }
        case arithmetic_mode::div : { std::cout << " / "; break; }
        default : { std::cout << " ? "; break; }
    }
    m_right->print();
    std::cout << " )";
}

} /* namespace ast */
} /* namespace mlang */