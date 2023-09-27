#pragma once

#include <vector>

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
        /* TODO */
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
    node_ptr left;
    node_ptr right;
    AssignmentOperationNode(node_ptr left, node_ptr right) : left(std::move(left)), right(std::move(right)) {}
    void print() const override {
        if (left != nullptr) left->print();
        std::cout << " = ";
        if (right != nullptr) right->print();
    }
};

} /* namespace lang */