#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class BreakNode : public Node {
public:
    BreakNode();
    ~BreakNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */