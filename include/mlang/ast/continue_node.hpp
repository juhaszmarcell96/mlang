#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class ContinueNode : public Node {
public:
    ContinueNode();
    ~ContinueNode () = default;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */