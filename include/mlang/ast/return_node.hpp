#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class ReturnNode : public Node {
private:
    node_ptr m_value;
public:
    ReturnNode(node_ptr value);
    ~ReturnNode () = default;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */