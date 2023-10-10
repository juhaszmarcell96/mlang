#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class MainNode : public Node {
private:
    std::vector<node_ptr> m_nodes;
public:
    MainNode() : Node(ast_node_types::main) {}
    ~MainNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    Object execute (EnvStack& env) const override {
        for (auto& node : m_nodes) {
            node->execute(env);
        }
        /* return null TODO */
        return Object {};
    }
    void add_node (node_ptr node) {
        m_nodes.push_back(std::move(node));
    }
    void print () const override {
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
    }
};

} /* namespace mlang */