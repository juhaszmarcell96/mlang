#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/array.hpp"

namespace mlang {
namespace ast {

class DeclarationOperationNode : public Node {
private:
    std::string m_var_name;
public:
    DeclarationOperationNode(const std::string& var_name);
    ~DeclarationOperationNode () = default;
    const std::string& get_var_name () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

class DeclAndInitOperationNode : public Node {
private:
    std::string m_var_name;
    node_ptr m_right;
public:
    DeclAndInitOperationNode(const std::string& var_name, node_ptr right);
    ~DeclAndInitOperationNode () = default;
    const std::string& get_var_name () const;
    const Node* const get_right () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */