#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class WhileStatementNode : public Node {
private:
    node_ptr m_condition;
    node_ptr m_body;
public:
    WhileStatementNode();
    ~WhileStatementNode () = default;
    object::Object execute (script::EnvStack& env) const override;
    void set_condition (node_ptr condition);
    void set_body (node_ptr body);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */