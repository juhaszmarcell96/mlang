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
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        for (auto& node : m_nodes) {
            node->execute(env);
        }
        /* return null TODO */
        return nullptr;
    }
    void add_node (node_ptr node) override {
        m_nodes.push_back(std::move(node));
    }
    Node* get_parent () override {
        throw unexpected_error{"a node of type 'MainNode' has no parent"};
        return nullptr;
    }
    void print () const override {
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
    }
};

} /* namespace mlang */