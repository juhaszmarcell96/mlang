#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class MemberAccessNode : public Node {
private:
    node_ptr m_lhs;
    std::string m_member_name;
public:
    MemberAccessNode(node_ptr lhs, const std::string& member_name);
    ~MemberAccessNode () = default;
    object::Object execute (script::EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */