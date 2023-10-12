#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

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
    BinaryArithmeticNode(node_ptr left, node_ptr right, arithmetic_mode mode);
    virtual ~BinaryArithmeticNode () = default;
    const Node* const get_left () const;
    const Node* const get_right () const;
    arithmetic_mode get_mode () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */