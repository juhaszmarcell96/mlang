#pragma once

#include "mlang/ast/node.hpp"
#include "mlang/ast/function.hpp"

namespace mlang {
namespace ast {

class FunctionDeclNode : public Node, public Function {
private:
    std::string m_name;
    node_ptr m_body;
    std::vector<std::string> m_params;
public:
    FunctionDeclNode (const std::string& name);
    ~FunctionDeclNode () = default;
    void set_body (node_ptr body);
    void add_parameter (const std::string& param);
    object::Object execute (script::EnvStack& env) const override;
    object::Object call (std::vector<object::Object>& params) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */