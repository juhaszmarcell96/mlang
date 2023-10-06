#pragma once

#include <vector>
#include <iostream>

#include "mlang/value.hpp"
#include "mlang/environment.hpp"
#include "mlang/token.hpp"
#include "mlang/exception.hpp"

namespace mlang {

enum class ast_node_types {
    none,
    main,
    value,
    variable,
    unary_not,
    unary_minus,
    binary_add,
    binary_sub,
    binary_mul,
    binary_div,
    assignment,
    declaration,
    add_equal,
    sub_equal,
    div_equal,
    mul_equal,
    if_statement,
    for_statement,
    while_statement,
    end_statement,
    equality,
    inequality,
    greater,
    less,
    greater_equal,
    less_equal,
    print,
    break_node
};

enum class value_category {
    lvalue = 0,
    rvalue = 1
};

class Node;

typedef std::unique_ptr<Node> node_ptr;

class Node {
private:
    ast_node_types m_type { ast_node_types::none };
    value_category m_category = { value_category::lvalue };
public:
    Node (ast_node_types type) : m_type(type) {}
    Node (ast_node_types type, value_category category) : m_type(type), m_category(category) {}
    virtual ~Node () = default;
    virtual void execute (EnvStack& env, Value& return_val) = 0;
    virtual void add_node (node_ptr node) {
        throw unexpected_error{"cannot add nodes this node"};
    }
    virtual Node* get_parent () {
        throw unexpected_error{"this node has no parent"};
        return nullptr;
    }
    virtual void add_elif (node_ptr condition) {
        throw unexpected_error{"unexpected elif"};
    }
    virtual void add_else () {
        throw unexpected_error{"unexpected else"};
    }
    virtual void print () const = 0;

    ast_node_types get_type () const { return m_type; }

    value_category get_category () const { return m_category; }
    void set_category (value_category category) { m_category = category; }
};

} /* namespace mlang */