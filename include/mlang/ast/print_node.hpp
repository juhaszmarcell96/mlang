#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class PrintNode : public Node {
private:
    std::string m_rule;
    std::vector<node_ptr> m_args;
    mutable std::size_t m_index { 0 };

    std::string get_integer (EnvStack& env) const;
    std::string get_string (EnvStack& env) const;
    std::string get_float (EnvStack& env) const;
    std::string get_bool (EnvStack& env) const;
public:
    PrintNode();
    ~PrintNode () = default;
    object::Object execute (EnvStack& env) const override;
    void set_rule (const std::string& rule);
    void add_argument (node_ptr arg);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */