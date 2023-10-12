#include "mlang/ast/for_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

ForStatementNode::ForStatementNode() : Node(ast_node_types::for_statement) {}

object::Object ForStatementNode::execute (script::EnvStack& env) const {
    env.enter_scope();
    /* assignments */
    if (m_initialization) { m_initialization->execute(env); }
    while (true) {
        env.enter_scope();
        /* check tests */
        object::Object test_val = m_test->execute(env);
        if (!(test_val.is_true())) {
            env.exit_scope();
            break;
        }
        try {
            /* execute scope */
            m_body->execute(env);
        }
        catch (const Break& e) {
            /* handle break */
            /* break the loop */
            env.exit_scope();
            break;
        }
        catch (const Continue& e) {
            /* handle continue */
            /* nothing to do, we carry on with the updates */
        }
        catch (const Return& e) {
            /* handle return -> exit the scope and throw it further, it is not ours to handle */
            env.exit_scope();
            env.exit_scope();
            throw;
        }
        /* do updates */
        if (m_update) { m_update->execute(env); }
        env.exit_scope();
    }
    env.exit_scope();
    return object::Object{};
}

void ForStatementNode::set_initialization (node_ptr initialization) { m_initialization = std::move(initialization); }

void ForStatementNode::set_test (node_ptr test) { m_test = std::move(test); }

void ForStatementNode::set_update (node_ptr update) { m_update = std::move(update); }

void ForStatementNode::set_body (node_ptr body) { m_body = std::move(body); }

void ForStatementNode::print () const {
    std::cout << "for ( ";
    if (m_initialization) { m_initialization->print(); }
    std::cout << "; ";
    if (m_test) { m_test->print(); }
    std::cout << "; ";
    if (m_update) { m_update->print(); }
    std::cout << ")" << std::endl;
    m_body->print();
}

} /* namespace ast */
} /* namespace mlang */