#include "mlang/ast/func_decl_node.hpp"
#include "mlang/ast/exception.hpp"
#include "mlang/function.hpp"

namespace mlang {
namespace ast {

FunctionDeclNode::FunctionDeclNode (const std::string& name) : Node(ast_node_types::func_decl), m_name(name) {}

void FunctionDeclNode::set_body (node_ptr body) { m_body = std::move(body); }

void FunctionDeclNode::add_parameter (const std::string& param) { m_params.push_back(param); }

object::Object FunctionDeclNode::execute (EnvStack& env) const {
    env.declare_function(m_name, this);
    return object::Object{};
}

object::Object FunctionDeclNode::call (std::vector<object::Object>& params) const {
    // function should be able to call other function and use global variables
    /* check if the parameters have the same length, none of them are nullptr and have the same type */
    /* same with the return value */
    if (params.size() != m_params.size()) {
        throw RuntimeError{ "function " + m_name + " expects " + std::to_string(m_params.size()) + " parameters but got " + std::to_string(params.size()) };
    }
    EnvStack env {};
    for (std::size_t i = 0; i < params.size(); ++i) {
        env.declare_variable(m_params[i], params[i].get_typename());
        env.get_variable(m_params[i]).assign(params[i]);
    }
    try {
        m_body->execute(env);
    }
    catch (const Break& e) {
        /* handle break */
        /* should not have gotten a break exception */
        throw RuntimeError{ "invalid 'break' in function " + m_name };
    }
    catch (const Continue& e) {
        /* should not have gotten a continue exception */
        throw RuntimeError{ "invalid 'continue' in function " + m_name };
    }
    catch (const Return& e) {
        /* handle  return */
        return e.get_value();
    }
    return object::Object {};
}

void FunctionDeclNode::print () const {
    /* if */
    std::cout << "function " << m_name << " ( ";
    for (std::size_t i = 0; i < m_params.size(); ++i) {
        std::cout << m_params[i];
        if (i < m_params.size() - 1) { std::cout << ", "; }
    }
    std::cout << " )" << std::endl;
    m_body->print();
}

} /* namespace ast */
} /* namespace mlang */