#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class VariableNode : public Node {
private:
    std::string m_var_name;
public:
    VariableNode(const std::string& var_name);
    ~VariableNode () = default;
    const std::string& get_var_name () const;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */