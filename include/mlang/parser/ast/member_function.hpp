#pragma once

#include "mlang/parser/ast/node.hpp"
#include "mlang/parser/ast/func_decl_node.hpp"
#include "mlang/function.hpp"

namespace mlang {

class MemberFunctionNode : public Node {
private:
    node_ptr m_lhs;
    std::string m_func_name;
    std::vector<node_ptr> m_params;
public:
    MemberFunctionNode(node_ptr lhs, const std::string& func_name) : Node(ast_node_types::member_func), m_lhs(lhs), m_func_name(func_name) {}
    ~MemberFunctionNode () = default;
    const std::vector<node_ptr>& get_params () const { return m_params; }
    Object execute (EnvStack& env) const override {
        std::vector<Object> params;
        for (const node_ptr& node : m_params) {
            params.push_back(node->execute(env));
        }
        Object lhs = m_lhs->execute(env);
        return lhs.call(m_func_name, params);
    }
    void add_parameter (node_ptr param) { m_params.push_back(std::move(param)); }
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