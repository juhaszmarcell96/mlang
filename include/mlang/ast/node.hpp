#pragma once

#include <vector>
#include <iostream>

#include "mlang/object/object.hpp"
#include "mlang/script/environment.hpp"
#include "mlang/exception.hpp"

namespace mlang {
namespace ast {

enum class ast_node_types {
    none,
    main,
    value,
    array,
    variable,
    unary_not,
    unary_minus,
    prefix,
    postfix,
    binary_arith,
    assignment,
    declaration,
    logic,
    if_statement,
    for_statement,
    while_statement,
    comparison,
    print,
    break_node,
    continue_node,
    return_node,
    exit_node,
    func_decl,
    func_call,
    subscript,
    member_func,
    block
};

class Node;

typedef std::unique_ptr<Node> node_ptr;

class Node {
private:
    ast_node_types m_type { ast_node_types::none };
public:
    Node (ast_node_types type) : m_type(type) {}
    virtual ~Node () = default;
    virtual object::Object execute (script::EnvStack& env) const = 0;
    virtual void print () const = 0;

    ast_node_types get_type () const { return m_type; }
};

} /* namespace ast */
} /* namespace mlang */