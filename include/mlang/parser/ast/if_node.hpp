#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class IfStatementNode : public Node {
private:
    node_ptr m_if_condition;
    node_ptr m_if_body;

    std::vector<node_ptr> m_elif_conditions;
    std::vector<node_ptr> m_elif_bodies;

    bool m_else_defined { false };
    node_ptr m_else_body;

    node_ptr* m_active_branch { nullptr };
public:
    IfStatementNode() : Node(ast_node_types::if_statement) {
        m_active_branch = &m_if_body;
    }
    ~IfStatementNode () = default;
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        env.enter_scope();
        try {
            /* if */
            std::shared_ptr<Object> cond_val = m_condition->execute(env);
            if (!cond_val) throw RuntimeError{"if statement condition returned null"};
            if (cond_val->is_true()) {
                m_if_body->execute(env);
                env.exit_scope();
                return nullptr;
            }
            /* elif */
            for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
                cond_val = m_elif_conditions[i]->execute(env);
                if (!cond_val) throw RuntimeError{"elif statement condition returned null"};
                if (cond_val->is_true()) {
                    m_elif_bodies[i]->execute(env);
                    env.exit_scope();
                    return nullptr;
                }
            }
            /* else */
            if (m_else_defined) {
                m_else_body->execute(env);
                env.exit_scope();
                return nullptr;
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
        return nullptr;
    }
    void set_if_condition (node_ptr condition) {
        m_if_condition = std::move(condition);
    }
    void add_block (node_ptr block) {
        *m_active_branch = std::move(block);
    }
    void add_elif_condition (node_ptr condition) {
        m_elif_conditions.push_back(std::move(condition));
        m_elif_bodies.push_back(node_ptr{});
        m_active_branch = &(m_elif_bodies.back());
    }
    void add_else () {
        m_else_defined = true;
        m_active_branch = &m_else_body;
    }
    void print () const override {
        /* if */
        std::cout << "if ( ";
        m_if_condition->print();
        std::cout << " )" << std::endl;
        m_if_body->print();
        /* elif */
        for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
            std::cout << "elif ( ";
            m_elif_conditions[i]->print();
            std::cout << " )" << std::endl;
            m_elif_bodies[i]->print();
        }
        /* else */
        if (m_else_defined) {
            std::cout << "else" << std::endl;
            m_else_body->print();
        }
    }
};

} /* namespace mlang */