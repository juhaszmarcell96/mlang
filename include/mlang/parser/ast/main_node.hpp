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
    void execute (Environment& env, Value& return_val) override {
        for (auto& node : m_nodes) {
            node->execute(env, return_val);
        }
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