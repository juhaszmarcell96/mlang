#include "mlang/ast/if_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

IfStatementNode::IfStatementNode() : Node(ast_node_types::if_statement) {
    m_active_branch = &m_if_body;
}

object::Object IfStatementNode::execute (script::EnvStack& env) const {
    env.enter_scope();
    try {
        /* if */
        object::Object cond_val = m_if_condition->execute(env);
        if (cond_val.is_true()) {
            m_if_body->execute(env);
            env.exit_scope();
            return object::Object {};
        }
        /* else if */
        for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
            cond_val = m_elif_conditions[i]->execute(env);
            if (cond_val.is_true()) {
                m_elif_bodies[i]->execute(env);
                env.exit_scope();
                return object::Object {};
            }
        }
        /* else */
        if (m_else_defined) {
            m_else_body->execute(env);
            env.exit_scope();
            return object::Object {};
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
    env.exit_scope();
    return object::Object {};
}

void IfStatementNode::set_if_condition (node_ptr condition) {
    m_if_condition = std::move(condition);
}

void IfStatementNode::add_block (node_ptr block) {
    *m_active_branch = std::move(block);
}

void IfStatementNode::add_elif_condition (node_ptr condition) {
    m_elif_conditions.push_back(std::move(condition));
    m_elif_bodies.push_back(node_ptr{});
    m_active_branch = &(m_elif_bodies.back());
}

void IfStatementNode::add_else () {
    m_else_defined = true;
    m_active_branch = &m_else_body;
}

void IfStatementNode::print () const {
    /* if */
    std::cout << "if ( ";
    m_if_condition->print();
    std::cout << " )" << std::endl;
    m_if_body->print();
    /* elif */
    for (std::size_t i = 0; i < m_elif_conditions.size(); ++i) {
        std::cout << "else if ( ";
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

} /* namespace ast */
} /* namespace mlang */