#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"
#include "mlang/function.hpp"

namespace mlang {

class FunctionDeclNode : public Node {
private:
    std::string m_name;
    node_ptr m_body;
    std::vector<std::string> m_params;
public:
    FunctionDeclNode (const std::string& name) : Node(ast_node_types::func_decl), m_name(name) {}
    ~FunctionDeclNode () = default;
    void set_body (node_ptr body) { m_body = std::move(body); }
    void add_parameter (const std::string& param) { m_params.push_back(param); }
    Object execute (EnvStack& env) const override {
        env.declare_function(m_name, this);
        return Object{};
    }
    Object call (std::vector<Object>& params) const {
        // function should be able to call other function and use global variables
        /* check if the parameters have the same length, none of them are nullptr and have the same type */
        /* same with the return value */
        if (params.size() != m_params.size()) {
            throw RuntimeError{ "function " + m_name + " expects " + std::to_string(m_params.size()) + " parameters but got " + std::to_string(params.size()) };
        }
        EnvStack env {};
        for (std::size_t i = 0; i < params.size(); ++i) {
            env.declare_variable(m_params[i].name, params[i].get_typename());
            env.set_variable(m_params[i].name, params[i]);
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
        return Object {};
    }
    void print () const override {
        /* if */
        std::cout << "function " << m_name << " ( ";
        for (std::size_t i = 0; i < m_params.size(); ++i) {
            std::cout << m_params[i];
            if (i < m_params.size() - 1) { std::cout << ", "; }
        }
        std::cout << " )" << std::endl;
        m_body->print();
    }
};

} /* namespace mlang */