#include "mlang/ast/assignment.hpp"

namespace mlang {
namespace ast {

AssignmentNode::AssignmentNode(node_ptr left, node_ptr right, assignment_mode mode) : Node(ast_node_types::assignment),
                                                                                      m_left(std::move(left)),
                                                                                      m_right(std::move(right)),
                                                                                      m_mode(mode) {}

const Node* const AssignmentNode::get_left () const { return m_left.get(); }

const Node* const AssignmentNode::get_right () const { return m_right.get(); }

assignment_mode AssignmentNode::get_mode () const { return m_mode; }

object::Object AssignmentNode::execute (script::EnvStack& env) const {
    object::Object lhs = m_left->execute(env);
    object::Object rhs = m_right->execute(env);
    switch (m_mode) {
        case assignment_mode::simple : { lhs.assign(rhs); break; }
        case assignment_mode::add    : { lhs.operator_add_equal(rhs); break; }
        case assignment_mode::sub    : { lhs.operator_sub_equal(rhs); break; }
        case assignment_mode::mul    : { lhs.operator_mul_equal(rhs); break; }
        case assignment_mode::div    : { lhs.operator_div_equal(rhs); break; }
        default : { throw RuntimeError{"invalid assignment operator type"}; }
    }
    return object::Object {};
}

void AssignmentNode::print () const {
    std::cout << "( ";
    m_left->print();
    switch (m_mode) {
        case assignment_mode::simple : { std::cout << " = "; break; }
        case assignment_mode::add    : { std::cout << " += "; break; }
        case assignment_mode::sub    : { std::cout << " -= "; break; }
        case assignment_mode::mul    : { std::cout << " *= "; break; }
        case assignment_mode::div    : { std::cout << " /= "; break; }
        default : { std::cout << " ? "; break; }
    }
    m_right->print();
    std::cout << " )";
}

} /* namespace ast */
} /* namespace mlang */