#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class ArrayNode : public Node {
private:
    std::vector<node_ptr> m_elements;
public:
    ArrayNode();
    ~ArrayNode () = default;
    object::Object execute (EnvStack& env) const override;
    void add_element (node_ptr elem);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */