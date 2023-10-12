#include "mlang/ast/while_node.hpp"
#include "mlang/ast/exception.hpp"

namespace mlang {
namespace ast {

WhileStatementNode::WhileStatementNode() : Node(ast_node_types::while_statement) {}

object::Object WhileStatementNode::execute (script::EnvStack& env) const {
    env.enter_scope();
    while (true) {
        object::Object cond_val = m_condition->execute(env);
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
    return object::Object{};
}

void WhileStatementNode::set_condition (node_ptr condition) { m_condition = std::move(condition); }

void WhileStatementNode::set_body (node_ptr body) { m_body = std::move(body); }

void WhileStatementNode::print () const {
    /* if */
    std::cout << "while (";
    m_condition->print();
    std::cout << ");" << std::endl;
    m_body->print();
}

} /* namespace ast */
} /* namespace mlang */