#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class ValueNode : public Node {
private:
    object::Object m_value;
public:
    ValueNode(object::Object value);
    ~ValueNode () = default;
    const object::Object& get_value () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */