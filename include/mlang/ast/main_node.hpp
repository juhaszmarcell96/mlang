#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class MainNode : public Node {
private:
    std::vector<node_ptr> m_nodes;
public:
    MainNode();
    ~MainNode () = default;
    const std::vector<node_ptr>& get_nodes () const;
    object::Object execute (EnvStack& env) const override;
    void add_node (node_ptr node);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */