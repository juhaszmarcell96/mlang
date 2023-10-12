#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class MemberFunctionNode : public Node {
private:
    node_ptr m_lhs;
    std::string m_func_name;
    std::vector<node_ptr> m_params;
public:
    MemberFunctionNode(node_ptr lhs, const std::string& func_name);
    ~MemberFunctionNode () = default;
    const std::vector<node_ptr>& get_params () const;
    object::Object execute (script::EnvStack& env) const override;
    void add_parameter (node_ptr param);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */