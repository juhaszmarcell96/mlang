#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

enum class assignment_mode {
    simple,
    add,
    sub,
    mul,
    div
};
    
class AssignmentNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
    assignment_mode m_mode { assignment_mode::simple };
public:
    AssignmentNode(node_ptr left, node_ptr right, assignment_mode mode) : Node(ast_node_types::assignment),
                                                                          m_left(std::move(left)),
                                                                          m_right(std::move(right)),
                                                                          m_mode(mode) {}
    ~AssignmentNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    assignment_mode get_mode () const { return m_mode; }
    Object execute (EnvStack& env) const override {
        Object lhs = m_left->execute(env);
        Object rhs = m_right->execute(env);
        switch (m_mode) {
            case assignment_mode::simple : { lhs.assign(rhs, *(env.get_factory(rhs.get_typename()))); }
            case assignment_mode::add    : { lhs.operator_add_equal(rhs, *(env.get_factory(rhs.get_typename()))); }
            case assignment_mode::sub    : { lhs.operator_sub_equal(rhs, *(env.get_factory(rhs.get_typename()))); }
            case assignment_mode::mul    : { lhs.operator_mul_equal(rhs, *(env.get_factory(rhs.get_typename()))); }
            case assignment_mode::div    : { lhs.operator_div_equal(rhs, *(env.get_factory(rhs.get_typename()))); }
            default : { break; }
        }
        throw RuntimeError{"invalid assignment operator type"};
    }
    void print () const override {
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
};

} /* namespace mlang */