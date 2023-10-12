#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

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
    AssignmentNode(node_ptr left, node_ptr right, assignment_mode mode);
    ~AssignmentNode () = default;
    const Node* const get_left () const;
    const Node* const get_right () const;
    assignment_mode get_mode () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */