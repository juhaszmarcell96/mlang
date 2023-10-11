#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

class ForStatementNode : public Node {
private:
    node_ptr m_initialization;
    node_ptr m_test;
    node_ptr m_update;
    node_ptr m_body;
public:
    ForStatementNode();
    ~ForStatementNode () = default;
    object::Object execute (EnvStack& env) const override;
    void set_initialization (node_ptr initialization);
    void set_test (node_ptr test);
    void set_update (node_ptr update);
    void set_body (node_ptr body);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */