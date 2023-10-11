#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class ExitNode : public Node {
private:
    node_ptr m_value;
public:
    ExitNode(node_ptr value);
    ~ExitNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */