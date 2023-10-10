#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class WhileStatementNode : public Node {
private:
    node_ptr m_condition;
    node_ptr m_body;
public:
    WhileStatementNode() : Node(ast_node_types::while_statement) {}
    ~WhileStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    Object execute (EnvStack& env) const override {
        env.enter_scope();
        while (true) {
            Object cond_val = m_condition->execute(env);
            if (!cond_val.is_true()) { break; }
            try {
                /* execute scope */
                m_body->execute(env);
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
                env.exit_scope();
                throw;
            }
        }
        env.exit_scope();
        return nullptr;
    }
    void set_condition (node_ptr condition) { m_condition = std::move(condition); }
    void set_body (node_ptr body) { m_body = std::move(body); }
    void print () const override {
        /* if */
        std::cout << "while (";
        m_condition->print();
        std::cout << ");" << std::endl;
        m_body->print();
    }
};

} /* namespace mlang */