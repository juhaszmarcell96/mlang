#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

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
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        env.enter_scope();
        while (true) {
            std::shared_ptr<Object> cond_val;
            m_condition->execute(env, cond_val);
            if (!cond_val) throw RuntimeError{"if statement condition returned null"};
            if (!cond_val->is_true()) { break; }
            try {
                /* execute scope */
                for (auto& node : m_nodes) {
                    node->execute(env, return_val);
                }
            }
            catch (const Break& e) {
                /* handle break */
                break;
            }
            catch (const Continue& e) {
                /* handle continue */
                continue;
            }
            catch (const Return& e) {
                /* handle return -> exit the scope and throw it further, it is not ours to handle */
                /* TODO : how about function returns? How will those be handled? By exception? Or rather by returning from the function as AST? */
                env.exit_scope();
                throw;
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