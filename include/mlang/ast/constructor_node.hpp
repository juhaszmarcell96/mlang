#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class ConstructorNode : public Node {
private:
    std::string m_type_name;
    std::vector<node_ptr> m_arguments;
public:
    ConstructorNode(const std::string& type_name);
    ~ConstructorNode () = default;
    object::Object execute (script::EnvStack& env) const override;
    void add_argument (node_ptr argument);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */