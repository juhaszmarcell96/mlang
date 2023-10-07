#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class ForStatementNode : public Node {
private:
    std::vector<node_ptr> m_assignments;
    std::vector<node_ptr> m_tests;
    std::vector<node_ptr> m_updates;
    Node* m_parent_scope { nullptr };
    std::vector<node_ptr> m_nodes;
public:
    ForStatementNode(Node* parent_scope) : Node(ast_node_types::for_statement), m_parent_scope(parent_scope) {}
    ~ForStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        env.enter_scope();
        /* assignments */
        for (auto& node : m_assignments) {
            node->execute(env, return_val);
        }
        while (true) {
            /* check tests */
            for (auto& test : m_tests) {
                std::shared_ptr<Object> test_val;
                test->execute(env, test_val);
                if (!test_val) throw RuntimeError{"for loop condition returned null"};
                /* if a condition is not satisfied, break the loop */
                if (!test_val->is_true()) {
                    return;
                }
            }
            try {
                /* execute scope */
                for (auto& node : m_nodes) {
                    node->execute(env, return_val);
                }
            }
            catch (const Break& e) {
                /* handle break */
                /* break the loop */
                break;
            }
            catch (const Continue& e) {
                /* handle continue */
                /* nothing to do, we carry on with the updates */
            }
            catch (const Return& e) {
                /* handle return -> exit the scope and throw it further, it is not ours to handle */
                env.exit_scope();
                throw;
            }
            /* do updates */
            for (auto& node : m_updates) {
                node->execute(env, return_val);
            }
        }
        env.exit_scope();
    }
    Node* get_parent () override {
        return m_parent_scope;
    }
    void add_assignment (node_ptr assignment) {
        m_assignments.push_back(std::move(assignment));
    }
    void add_test (node_ptr test) {
        m_tests.push_back(std::move(test));
    }
    void add_update (node_ptr update) {
        m_updates.push_back(std::move(update));
    }
    void add_node (node_ptr node) override {
        m_nodes.push_back(std::move(node));
    }
    void print () const override {
        /* if */
        std::cout << "for ( ";
        for (std::size_t i = 0; i < m_assignments.size(); ++i) {
            m_assignments[i]->print();
            if (i < m_assignments.size() - 1) { std::cout << ", "; }
        }
        std::cout << "; ";
        for (std::size_t i = 0; i < m_tests.size(); ++i) {
            m_tests[i]->print();
            if (i < m_tests.size() - 1) { std::cout << ", "; }
        }
        std::cout << "; ";
        for (std::size_t i = 0; i < m_updates.size(); ++i) {
            m_updates[i]->print();
            if (i < m_updates.size() - 1) { std::cout << ", "; }
        }
        std::cout << ");" << std::endl;
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
    }
};

} /* namespace mlang */