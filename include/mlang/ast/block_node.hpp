#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class BlockNode : public Node {
private:
    std::vector<node_ptr> m_nodes;
public:
    BlockNode();
    ~BlockNode () = default;
    const std::vector<node_ptr>& get_nodes () const;
    object::Object execute (script::EnvStack& env) const override;
    void add_node (node_ptr node);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */