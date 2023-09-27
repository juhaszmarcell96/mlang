#pragma once

#include <vector>
#include <iostream>

#include "value.hpp"

namespace lang {

struct Node {
    virtual ~Node() = default;
    virtual void print() const = 0;
};

typedef std::unique_ptr<Node> node_ptr;

struct ValueNode : Node {
    Value value;
    explicit ValueNode(Value value) : value(value) {}
    void print() const override {
        if (value.get_type() == value_types::number) {
            std::cout << value.get_number();
        }
        if (value.get_type() == value_types::string) {
            std::cout << value.get_string();
        }
        /* TODO : array */
    }
};

struct BinaryAddOperationNode : Node {
    node_ptr left;
    node_ptr right;
    BinaryAddOperationNode(node_ptr left, node_ptr right) : left(std::move(left)), right(std::move(right)) {}
    void print() const override {
        std::cout << "( ";
        if (left != nullptr) left->print();
        std::cout << " + ";
        if (right != nullptr) right->print();
        std::cout << " )";
    }
};

struct BinarySubOperationNode : Node {
    node_ptr left;
    node_ptr right;
    BinarySubOperationNode(node_ptr left, node_ptr right) : left(std::move(left)), right(std::move(right)) {}
    void print() const override {
        std::cout << "( ";
        if (left != nullptr) left->print();
        std::cout << " - ";
        if (right != nullptr) right->print();
        std::cout << " )";
    }
};

struct BinaryMulOperationNode : Node {
    node_ptr left;
    node_ptr right;
    BinaryMulOperationNode(node_ptr left, node_ptr right) : left(std::move(left)), right(std::move(right)) {}
    void print() const override {
        std::cout << "( ";
        if (left != nullptr) left->print();
        std::cout << " * ";
        if (right != nullptr) right->print();
        std::cout << " )";
    }
};

struct BinaryDivOperationNode : Node {
    node_ptr left;
    node_ptr right;
    BinaryDivOperationNode(node_ptr left, node_ptr right) : left(std::move(left)), right(std::move(right)) {}
    void print() const override {
        std::cout << "( ";
        if (left != nullptr) left->print();
        std::cout << " / ";
        if (right != nullptr) right->print();
        std::cout << " )";
    }
};

struct AssignmentOperationNode : Node {
    std::string name;
    node_ptr right;
    AssignmentOperationNode(const std::string& var_name, node_ptr right) : name(var_name), right(std::move(right)) {}
    void print() const override {
        std::cout << name << " = ";
        if (right != nullptr) right->print();
    }
};

struct DeclarationOperationNode : Node {
    token_types type;
    std::string name;
    DeclarationOperationNode(token_types var_type, const std::string& var_name) : type(var_type), name(var_name) {}
    void print() const override {
        switch (type) {
            case token_types::kw_number : {
                std::cout << "number ";
                break;
            }
            case token_types::kw_string : {
                std::cout << "string ";
                break;
            }
            case token_types::kw_array : {
                std::cout << "array ";
                break;
            }
        }
        std::cout << name << std::endl;
    }
};

} /* namespace lang */