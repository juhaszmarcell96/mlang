#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/func_decl_node.hpp"
#include "mlang/function.hpp"

namespace mlang {

class MemberFunctionNode : public Node {
private:
    std::string m_var_name;
    std::string m_func_name;
    std::vector<node_ptr> m_params;
public:
    MemberFunctionNode(const std::string& var_name, const std::string& func_name) : Node(ast_node_types::member_func), m_var_name(var_name), m_func_name(func_name) {}
    ~MemberFunctionNode () = default;
    const std::vector<node_ptr>& get_params () const { return m_params; }
    void execute (EnvStack& env, std::shared_ptr<Object>& return_val) override {
        std::vector<std::shared_ptr<Object>> params;
        for (node_ptr& node : m_params) {
            std::shared_ptr<Object> ret_val;
            node->execute(env, ret_val);
            params.push_back(ret_val);
        }
        return_val = env.get_variable(m_var_name)->call(m_func_name, params);
    }
    void add_parameter (node_ptr param) {
        m_params.push_back(std::move(param));
    }
    void print () const override {
        std::cout << m_var_name << "." << m_func_name << " ( ";
        for (std::size_t i = 0; i < m_params.size(); ++i) {
            m_params[i]->print();
            if (i < m_params.size() - 1) { std::cout << ", "; }
        }
        std::cout << " );" << std::endl;
    }
};

} /* namespace mlang */