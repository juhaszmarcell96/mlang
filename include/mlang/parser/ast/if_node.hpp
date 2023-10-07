#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class IfStatementNode : public Node {
private:
    node_ptr m_condition;
    Node* m_parent_scope { nullptr };
    std::vector<node_ptr> m_nodes;

    std::vector<node_ptr> m_elif_conditions;
    std::vector<std::vector<node_ptr>> m_elif_nodes;

    bool m_else_defined { false };
    std::vector<node_ptr> m_else_nodes;

    std::vector<node_ptr>* m_active_branch { nullptr };
public:
    IfStatementNode(node_ptr condition, Node* parent_scope) : Node(ast_node_types::if_statement), m_condition(std::move(condition)), m_parent_scope(parent_scope) {
        m_active_branch = &m_nodes;
    }
    ~IfStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    const Node* const get_condition () const { return m_condition.get(); }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        env.enter_scope();
        try {
            /* if */
            std::shared_ptr<Object> cond_val;
            m_condition->execute(env, cond_val);
            if (!cond_val) throw RuntimeError{"if statement condition returned null"};
            if (cond_val->is_true()) {
                for (auto& node : m_nodes) {
                    node->execute(env, return_val);
                }
                env.exit_scope();
                return;
            }
            /* elif */
            for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
                cond_val = nullptr;
                m_elif_conditions[i]->execute(env, cond_val);
                if (!cond_val) throw RuntimeError{"elif statement condition returned null"};
                if (cond_val->is_true()) {
                    for (auto& node : m_elif_nodes[i]) {
                        node->execute(env, return_val);
                    }
                    env.exit_scope();
                    return;
                }
            }
            /* else */
            if (m_else_defined) {
                for (auto& node : m_else_nodes) {
                    node->execute(env, return_val);
                }
                env.exit_scope();
                return;
            }
        }
        catch (const Break& e) {
            /* handle break */
            /* this is not ours to handle, exit the scope and throw it further */
            env.exit_scope();
            throw;
        }
        catch (const Continue& e) {
            /* handle continue */
            /* this is not ours to handle, exit the scope and throw it further */
            env.exit_scope();
            throw;
        }
        catch (const Return& e) {
            /* handle  return */
            /* this is not ours to handle, exit the scope and throw it further */
            env.exit_scope();
            throw;
        }
    }
    void add_node (node_ptr node) override {
        m_active_branch->push_back(std::move(node));
    }
    Node* get_parent () override {
        return m_parent_scope;
    }
    void add_elif (node_ptr condition) override {
        m_elif_conditions.push_back(std::move(condition));
        m_elif_nodes.push_back(std::vector<node_ptr>{});
        m_active_branch = &(m_elif_nodes.back());
    }
    void add_else () override {
        m_else_defined = true;
        m_active_branch = &m_else_nodes;
    }
    void print () const override {
        /* if */
        std::cout << "if ( ";
        m_condition->print();
        std::cout << " )" << std::endl;
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
        /* elif */
        for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
            std::cout << "elif ( ";
            m_elif_conditions[i]->print();
            std::cout << " )" << std::endl;
            for (auto& node : m_elif_nodes[i]) {
                node->print();
                std::cout << std::endl;
            }
        }
        /* else */
        if (m_else_defined) {
            std::cout << "else" << std::endl;
            for (auto& node : m_else_nodes) {
                node->print();
                std::cout << std::endl;
            }
        }
    }
};

} /* namespace mlang */