#include "mlang/ast/print_node.hpp"

namespace mlang {
namespace ast {

std::string PrintNode::get_integer (EnvStack& env) const {
    if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
    object::Object res = m_args[m_index]->execute(env);
    m_args[m_index]->print();
    ++m_index;
    return std::to_string(static_cast<int>(res.get_number()));
}

std::string PrintNode::get_string (EnvStack& env) const {
    if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
    object::Object res = m_args[m_index]->execute(env);
    ++m_index;
    return res.get_string();
}

std::string PrintNode::get_float (EnvStack& env) const {
    if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
    object::Object res = m_args[m_index]->execute(env);
    ++m_index;
    return std::to_string(res.get_number());
}

std::string PrintNode::get_bool (EnvStack& env) const {
    if (m_index >= m_args.size()) { throw syntax_error{"mismatch in print arguments"}; }
    object::Object res = m_args[m_index]->execute(env);
    ++m_index;
    return (res.is_true() ? "true" : "false");
}

PrintNode::PrintNode() : Node(ast_node_types::print) {}

object::Object PrintNode::execute (EnvStack& env) const {
    m_index = 0;
    std::string result;
    for  (std::size_t i = 0; i < m_rule.length(); ++i) {
        if (i == m_rule.length() - 1) {
            result.push_back(m_rule[i]);
            break;
        }
        if (m_rule[i] == '%') {
            switch (m_rule[i + 1]) {
                case 'd' : {
                    result += get_integer(env);
                    ++i;
                    break;
                }
                case 'f' : {
                    result += get_float(env);
                    ++i;
                    break;
                }
                case 'b' : {
                    result += get_bool(env);
                    ++i;
                    break;
                }
                case 's' : {
                    result += get_string(env);
                    ++i;
                    break;
                }
                case '%' : {
                    result.push_back(m_rule[i]);
                    ++i;
                    break;
                }
                default : {
                    result.push_back(m_rule[i]);
                    break;
                }
            }
        }
        else {
            result.push_back(m_rule[i]);
        }
    }
    std::cout << result << std::endl;
    return object::Object {};
}

void PrintNode::set_rule (const std::string& rule) { m_rule = rule; }

void PrintNode::add_argument (node_ptr arg) { m_args.push_back(std::move(arg)); }

void PrintNode::print () const {
    std::cout << "print(" + m_rule;
    for (const node_ptr& arg : m_args) {
        std::cout << ", ";
        arg->print();
    }
    std::cout << ")";
}

} /* namespace ast */
} /* namespace mlang */