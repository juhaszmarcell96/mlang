#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

enum class arithmetic_mode {
    add,
    sub,
    mul,
    div
};

class BinaryArithmeticNode : public Node {
protected:
    node_ptr m_left;
    node_ptr m_right;
    arithmetic_mode m_mode { arithmetic_mode::add };
public:
    BinaryArithmeticNode(node_ptr left, node_ptr right, arithmetic_mode mode) : Node(binary_arith),
                                                                                m_left(std::move(left)),
                                                                                m_right(std::move(right)),
                                                                                m_mode(mode) {}
    virtual ~BinaryArithmeticNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    arithmetic_mode get_mode () const { return m_mode; }
    Object execute (EnvStack& env) const override {
        Object lhs = m_left->execute(env);
        Object rhs = m_right->execute(env);
        switch (m_mode) {
            case arithmetic_mode::add : { return lhs.operator_binary_add(rhs); }
            case arithmetic_mode::sub : { return lhs.operator_binary_sub(rhs); }
            case arithmetic_mode::mul : { return lhs.operator_binary_mul(rhs); }
            case arithmetic_mode::div : { return lhs.operator_binary_div(rhs); }
            default : { break; }
        }
        throw RuntimeError{"invalid arithmetic operator type"};
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        switch (m_mode) {
            case arithmetic_mode::add : { std::cout << " + "; break; }
            case arithmetic_mode::sub : { std::cout << " - "; break; }
            case arithmetic_mode::mul : { std::cout << " * "; break; }
            case arithmetic_mode::div : { std::cout << " / "; break; }
            default : { std::cout << " ? "; break }
        }
        m_right->print();
        std::cout << " )";
    }
};

} /* namespace mlang */