#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/exception.hpp"

namespace mlang {
namespace ast {

class SubscriptNode : public Node {
private:
    node_ptr m_index;
    node_ptr m_lhs;
public:
    SubscriptNode(node_ptr lhs);
    ~SubscriptNode () = default;
    void set_index (node_ptr index);
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */