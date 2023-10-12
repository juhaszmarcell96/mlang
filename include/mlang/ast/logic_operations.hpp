#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

enum class logic_mode {
    logic_and,
    logic_or
};

class BinaryLogicNode : public Node {
protected:
    node_ptr m_left;
    node_ptr m_right;
    logic_mode m_mode { logic_mode::logic_and };
public:
    BinaryLogicNode(node_ptr left, node_ptr right, logic_mode mode);
    virtual ~BinaryLogicNode () = default;
    const Node* const get_left () const;
    const Node* const get_right () const;
    logic_mode get_mode () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */