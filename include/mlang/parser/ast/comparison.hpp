#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

enum class comparison_mode {
    equal,
    not_equal,
    greater,
    less,
    greater_equal,
    less_equal
};

class BinaryComparisonNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
    comparison_mode m_mode { comparison_mode::equal };
public:
    BinaryComparisonNode(node_ptr left, node_ptr right, comparison_mode mode) : Node(ast_node_types::comparison), 
                                                                                m_left(std::move(left)),
                                                                                m_right(std::move(right)),
                                                                                m_mode(comparison_mode) {}
    ~BinaryComparisonNode () = default;
    const Node* const get_left () const { return m_left.get(); }
    const Node* const get_right () const { return m_right.get(); }
    comparison_mode get_mode () const { return m_mode; }
    Object execute (EnvStack& env) const override {
        Object lhs = m_left->execute(env);
        Object rhs = m_right->execute(env);
        switch (m_mode) {
            case comparison_mode::equal         : { return lhs.operator_comparison_equal(rhs); }
            case comparison_mode::not_equal     : { return lhs.operator_comparison_not_equal(rhs); }
            case comparison_mode::greater       : { return lhs.operator_greater(rhs); }
            case comparison_mode::less          : { return lhs.operator_less(rhs); }
            case comparison_mode::greater_equal : { return lhs.operator_greater_equal(rhs); }
            case comparison_mode::less_equal    : { return lhs.operator_less_equal(rhs); }
            default : { break; }
        }
        throw RuntimeError{"invalid comparison operator type"};
    }
    void print () const override {
        std::cout << "( ";
        m_left->print();
        switch (m_mode) {
            case comparison_mode::equal         : { std::cout << " == "; break; }
            case comparison_mode::not_equal     : { std::cout << " != "; break; }
            case comparison_mode::greater       : { std::cout << " > "; break; }
            case comparison_mode::less          : { std::cout << " < "; break; }
            case comparison_mode::greater_equal : { std::cout << " >= "; break; }
            case comparison_mode::less_equal    : { std::cout << " <= "; break; }
            default : { std::cout << " ?? "; break }
        }
        m_right->print();
        std::cout << " )";
    }
};

} /* namespace mlang */