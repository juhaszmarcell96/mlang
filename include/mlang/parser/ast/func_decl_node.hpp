#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/exception.hpp"
#include "mlang/function.hpp"

namespace mlang {

struct FuncParam {
    std::string type;
    std::string name;
};

class FunctionDeclNode : public Node {
private:
    std::string m_name;
    Node* m_parent_scope { nullptr };
    std::vector<node_ptr> m_nodes;
    std::vector<FuncParam> m_params;
    std::string m_ret_type;
public:
    FunctionDeclNode(const std::string& name, Node* parent_scope) : Node(ast_node_types::func_decl), m_name(name), m_parent_scope(parent_scope) {}
    ~FunctionDeclNode () = default;
    const std::vector<node_ptr>& get_nodes () const { return m_nodes; }
    std::shared_ptr<Object> execute (EnvStack& env) const override {
        env.declare_function(m_name, this);
        return nullptr;
    }
    std::shared_ptr<Object> call (std::vector<std::shared_ptr<Object>>& params) const {
        // function should be able to call other function and use global variables
        /* check if the parameters have the same length, none of them are nullptr and have the same type */
        /* same with the return value */
        if (params.size() != m_params.size()) {
            throw RuntimeError{ "function " + m_name + " expects " + std::to_string(m_params.size()) + " parameters but got " + std::to_string(params.size()) };
        }
        EnvStack env {};
        for (std::size_t i = 0; i < params.size(); ++i) {
            if (params[i] == nullptr) {
                throw RuntimeError{ "parameter is null for function " + m_name };
            }
            if (params[i]->get_typename().compare(m_params[i].type) != 0) {
                throw RuntimeError{ "parameter type is invalid for function " + m_name };
            }
            env.declare_variable(m_params[i].name, m_params[i].type);
            env.set_variable(m_params[i].name, params[i]);
        }
        try {
            for (auto& node : m_nodes) {
                node->execute(env);
            }
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
        return nullptr;
    }
    void add_node (node_ptr node) override {
        m_nodes.push_back(std::move(node));
    }
    Node* get_parent () override {
        return m_parent_scope;
    }
    bool add_parameter (const std::string& type, const std::string& name) {
        for (const FuncParam param : m_params) {
            if (param.name.compare(name) == 0) {
                return false;
            }
        }
        m_params.push_back(FuncParam{.type=type, .name=name});
        return true;
    }
    void define_ret_type (const std::string& type) {
        m_ret_type = type;
    }
    void print () const override {
        /* if */
        std::cout << "function " << m_name << " ( ";
        for (std::size_t i = 0; i < m_params.size(); ++i) {
            std::cout << m_params[i].name;
            if (i < m_params.size() - 1) { std::cout << ", "; }
        }
        std::cout << " )" << std::endl;
        for (auto& node : m_nodes) {
            node->print();
            std::cout << std::endl;
        }
    }
};

} /* namespace mlang */