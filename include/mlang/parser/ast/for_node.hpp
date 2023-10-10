#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"

namespace mlang {

class ForStatementNode : public Node {
private:
    node_ptr m_initialization;
    node_ptr m_test;
    node_ptr m_update;
    node_ptr m_body;
public:
    ForStatementNode() : Node(ast_node_types::for_statement) {}
    ~ForStatementNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    Object execute (EnvStack& env) const override {
        env.enter_scope();
        /* assignments */
        if (m_initialization) { m_initialization->execute(env); }
        while (true) {
            /* check tests */
            std::shared_ptr<Object> test_val;
            if (m_test) {
                test_val = m_test->execute(env);
                if (!test_val) throw RuntimeError{"for loop test returned null"};
                if (!(test_val->is_true())) {
                    return Object{};
                }
            }
            try {
                /* execute scope */
                m_body->execute(env);
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
            if (m_update) { m_update->execute(env); }
        }
        env.exit_scope();
        return Object{};
    }
    void set_initialization (node_ptr initialization) { m_initialization = std::move(initialization); }
    void set_test (node_ptr test) { m_test = std::move(test); }
    void set_update (node_ptr update) { m_update = std::move(update); }
    void set_body (node_ptr body) { m_body = std::move(body); }
    void print () const override {
        std::cout << "for ( ";
        if (m_initialization) { m_initialization->print(); }
        std::cout << "; ";
        if (m_test) { m_test->print(); }
        std::cout << "; ";
        if (m_update) { m_update->print(); }
        std::cout << ")" << std::endl;
        m_body->print();
    }
};

} /* namespace mlang */