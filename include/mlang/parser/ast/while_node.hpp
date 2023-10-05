#pragma once

#include "mlang/parser/ast/node.hpp"

namespace mlang {

class WhileStatementNode : public Node {
private:
    node_ptr m_condition;
    Node* m_parent_scope { nullptr };
    std::vector<node_ptr> m_nodes;
public:
    WhileStatementNode(node_ptr condition, Node* parent_scope) : Node(ast_node_types::while_statement), m_condition(std::move(condition)), m_parent_scope(parent_scope) {}
    ~WhileStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    void execute (EnvStack& env, Value& return_val) override {
        env.enter_scope();
        while (true) {
            Value condition_val {};
            m_condition->execute(env, condition_val);
            if (!condition_val) { break; }
            /* execute scope */
            for (auto& node : m_nodes) {
                node->execute(env, return_val);
            }
        }
        env.exit_scope();
    }
    Node* get_parent () override {
        return m_parent_scope;
    }
    void add_node (node_ptr node) override {
        m_nodes.push_back(std::move(node));
    }
    void print () const override {
        /* if */
        std::cout << "while (";
        m_condition->print();
        std::cout << ");" << std::endl;
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
    }
};

} /* namespace mlang */