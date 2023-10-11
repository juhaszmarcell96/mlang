#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class IfStatementNode : public Node {
private:
    node_ptr m_if_condition;
    node_ptr m_if_body;

    std::vector<node_ptr> m_elif_conditions;
    std::vector<node_ptr> m_elif_bodies;

    bool m_else_defined { false };
    node_ptr m_else_body;

    node_ptr* m_active_branch { nullptr };
public:
    IfStatementNode();
    ~IfStatementNode () = default;
    object::Object execute (EnvStack& env) const override;
    void set_if_condition (node_ptr condition);
    void add_block (node_ptr block);
    void add_elif_condition (node_ptr condition);
    void add_else ();
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */