#pragma once

#include <vector>
#include <iostream>

#include "mlang/object/object.hpp"
#include "mlang/environment.hpp"
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
    break_node,
    return_node,
    func_decl,
    func_call,
    indexing_node,
    member_func
};

class Node;

typedef std::unique_ptr<Node> node_ptr;

class Node {
private:
    ast_node_types m_type { ast_node_types::none };
public:
    Node (ast_node_types type) : m_type(type) {}
    virtual ~Node () = default;
    virtual void execute (EnvStack& env, std::shared_ptr<Object>& return_val) = 0;
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
    
    virtual bool is_lvalue () const {
        return false;
    }
};

} /* namespace mlang */