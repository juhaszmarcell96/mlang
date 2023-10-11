#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class FunctionCallNode : public Node {
private:
    std::string m_name;
    std::vector<node_ptr> m_params;
public:
    FunctionCallNode(const std::string& name);
    ~FunctionCallNode () = default;
    const std::vector<node_ptr>& get_params () const;
    object::Object execute (EnvStack& env) const override;
    void add_parameter (node_ptr param);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */