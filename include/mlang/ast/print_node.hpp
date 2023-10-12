#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

class PrintNode : public Node {
private:
    std::string m_rule;
    std::vector<node_ptr> m_args;
    mutable std::size_t m_index { 0 };

    std::string get_integer (script::EnvStack& env) const;
    std::string get_string (script::EnvStack& env) const;
    std::string get_float (script::EnvStack& env) const;
    std::string get_bool (script::EnvStack& env) const;
public:
    PrintNode();
    ~PrintNode () = default;
    object::Object execute (script::EnvStack& env) const override;
    void set_rule (const std::string& rule);
    void add_argument (node_ptr arg);
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */