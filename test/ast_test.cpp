#include <gtest/gtest.h>

#include <string>

#include "mlang/object/object.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/boolean.hpp"
#include "mlang/object/none.hpp"
#include "mlang/parser/ast.hpp"

TEST(ASTTest, Test0) {
    mlang::EnvStack env {};
    std::shared_ptr<mlang::Object> ret_val;
    std::string var_name = "num";

    ASSERT_EQ(env.has_variable(var_name), false);

    mlang::DeclarationOperationNode node { var_name };
    node.execute(env);

    ASSERT_EQ(env.has_variable(var_name), true);
    ASSERT_EQ(env.get_variable(var_name)->get_typename(), mlang::None::type_name);

    std::shared_ptr<mlang::Number> num_ptr = std::make_shared<mlang::Number>(5);
    std::unique_ptr<mlang::ValueNode> rhs = std::make_unique<mlang::ValueNode>(num_ptr);
    mlang::AssignmentOperationNode assignment { var_name, std::move(rhs) };
    assignment.execute(env);

    ASSERT_EQ(env.has_variable(var_name), true);
    ASSERT_EQ(env.get_variable(var_name)->get_typename(), mlang::Number::type_name);
    ASSERT_EQ(env.get_variable(var_name)->get_number(), 5);
}
/*
TEST(ASTTest, Test1) {
    mlang::EnvStack env {};
    mlang::Value ret_val {};

    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);

    mlang::DeclarationOperationNode declaration_a {mlang::value_types::number, "a"};
    mlang::DeclarationOperationNode declaration_b {mlang::value_types::number, "b"};
    declaration_a.execute(env, ret_val);
    declaration_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 0);
    ASSERT_EQ(env.get_variable("b")->get_number(), 0);

    std::unique_ptr<mlang::ValueNode> rhs_a = std::make_unique<mlang::ValueNode>(mlang::Value { 5 });
    std::unique_ptr<mlang::ValueNode> rhs_b = std::make_unique<mlang::ValueNode>(mlang::Value { 7 });
    mlang::AssignmentOperationNode assignment_a { "a", std::move(rhs_a) };
    mlang::AssignmentOperationNode assignment_b { "b", std::move(rhs_b) };
    assignment_a.execute(env, ret_val);
    assignment_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 5);
    ASSERT_EQ(env.get_variable("b")->get_number(), 7);
}

TEST(ASTTest, Test2) {
    mlang::EnvStack env {};
    mlang::Value ret_val {};

    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);

    mlang::DeclarationOperationNode declaration_a {mlang::value_types::number, "a"};
    mlang::DeclarationOperationNode declaration_b {mlang::value_types::number, "b"};
    declaration_a.execute(env, ret_val);
    declaration_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 0);
    ASSERT_EQ(env.get_variable("b")->get_number(), 0);

    std::unique_ptr<mlang::ValueNode> rhs_a = std::make_unique<mlang::ValueNode>(mlang::Value { 5 });
    mlang::AssignmentOperationNode assignment_a { "a", std::move(rhs_a) };
    assignment_a.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 5);
    
    std::unique_ptr<mlang::VariableNode> lhs_b = std::make_unique<mlang::VariableNode>("a");
    std::unique_ptr<mlang::ValueNode> rhs_b = std::make_unique<mlang::ValueNode>(mlang::Value { 7 });
    std::unique_ptr<mlang::BinaryAddOperationNode> addition = std::make_unique<mlang::BinaryAddOperationNode>(std::move(lhs_b), std::move(rhs_b));
    mlang::AssignmentOperationNode assignment_b { "b", std::move(addition) };
    assignment_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_number(), 12);
}

TEST(ASTTest, Test3) {
    mlang::EnvStack env {};
    mlang::Value ret_val {};

    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);

    mlang::DeclarationOperationNode declaration_a {mlang::value_types::string, "a"};
    mlang::DeclarationOperationNode declaration_b {mlang::value_types::string, "b"};
    declaration_a.execute(env, ret_val);
    declaration_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::string);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::string);
    ASSERT_EQ(env.get_variable("a")->get_string(), "");
    ASSERT_EQ(env.get_variable("b")->get_string(), "");

    std::unique_ptr<mlang::ValueNode> rhs_a = std::make_unique<mlang::ValueNode>(mlang::Value { "asd" });
    mlang::AssignmentOperationNode assignment_a { "a", std::move(rhs_a) };
    assignment_a.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::string);
    ASSERT_EQ(env.get_variable("a")->get_string(), "asd");
    
    std::unique_ptr<mlang::VariableNode> lhs_b = std::make_unique<mlang::VariableNode>("a");
    std::unique_ptr<mlang::ValueNode> rhs_b = std::make_unique<mlang::ValueNode>(mlang::Value { "fgh" });
    std::unique_ptr<mlang::BinaryAddOperationNode> addition = std::make_unique<mlang::BinaryAddOperationNode>(std::move(lhs_b), std::move(rhs_b));
    mlang::AssignmentOperationNode assignment_b { "b", std::move(addition) };
    assignment_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::string);
    ASSERT_EQ(env.get_variable("b")->get_string(), "asdfgh");
}

TEST(ASTTest, Test4) {
    mlang::EnvStack env {};
    mlang::Value ret_val {};

    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);

    mlang::DeclarationOperationNode declaration_a {mlang::value_types::number, "a"};
    mlang::DeclarationOperationNode declaration_b {mlang::value_types::number, "b"};
    declaration_a.execute(env, ret_val);
    declaration_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 0);
    ASSERT_EQ(env.get_variable("b")->get_number(), 0);

    std::unique_ptr<mlang::ValueNode> rhs_a = std::make_unique<mlang::ValueNode>(mlang::Value { 5 });
    mlang::AssignmentOperationNode assignment_a { "a", std::move(rhs_a) };
    assignment_a.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 5);
    
    std::unique_ptr<mlang::VariableNode> lhs_b = std::make_unique<mlang::VariableNode>("a");
    std::unique_ptr<mlang::ValueNode> rhs_b = std::make_unique<mlang::ValueNode>(mlang::Value { 7 });
    std::unique_ptr<mlang::BinarySubOperationNode> subtraction = std::make_unique<mlang::BinarySubOperationNode>(std::move(lhs_b), std::move(rhs_b));
    mlang::AssignmentOperationNode assignment_b { "b", std::move(subtraction) };
    assignment_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_number(), -2);
}

TEST(ASTTest, Test5) {
    mlang::EnvStack env {};
    mlang::Value ret_val {};

    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);

    mlang::DeclarationOperationNode declaration_a {mlang::value_types::number, "a"};
    mlang::DeclarationOperationNode declaration_b {mlang::value_types::number, "b"};
    declaration_a.execute(env, ret_val);
    declaration_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 0);
    ASSERT_EQ(env.get_variable("b")->get_number(), 0);

    std::unique_ptr<mlang::ValueNode> rhs_a = std::make_unique<mlang::ValueNode>(mlang::Value { 5 });
    mlang::AssignmentOperationNode assignment_a { "a", std::move(rhs_a) };
    assignment_a.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 5);
    
    std::unique_ptr<mlang::VariableNode> lhs_b = std::make_unique<mlang::VariableNode>("a");
    std::unique_ptr<mlang::ValueNode> rhs_b = std::make_unique<mlang::ValueNode>(mlang::Value { 7 });
    std::unique_ptr<mlang::BinaryMulOperationNode> multiplication = std::make_unique<mlang::BinaryMulOperationNode>(std::move(lhs_b), std::move(rhs_b));
    mlang::AssignmentOperationNode assignment_b { "b", std::move(multiplication) };
    assignment_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_number(), 35);
}

TEST(ASTTest, Test6) {
    mlang::EnvStack env {};
    mlang::Value ret_val {};

    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);

    mlang::DeclarationOperationNode declaration_a {mlang::value_types::number, "a"};
    mlang::DeclarationOperationNode declaration_b {mlang::value_types::number, "b"};
    declaration_a.execute(env, ret_val);
    declaration_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 0);
    ASSERT_EQ(env.get_variable("b")->get_number(), 0);

    std::unique_ptr<mlang::ValueNode> rhs_a = std::make_unique<mlang::ValueNode>(mlang::Value { 10 });
    mlang::AssignmentOperationNode assignment_a { "a", std::move(rhs_a) };
    assignment_a.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 10);
    
    std::unique_ptr<mlang::VariableNode> lhs_b = std::make_unique<mlang::VariableNode>("a");
    std::unique_ptr<mlang::ValueNode> rhs_b = std::make_unique<mlang::ValueNode>(mlang::Value { 5 });
    std::unique_ptr<mlang::BinaryDivOperationNode> division = std::make_unique<mlang::BinaryDivOperationNode>(std::move(lhs_b), std::move(rhs_b));
    mlang::AssignmentOperationNode assignment_b { "b", std::move(division) };
    assignment_b.execute(env, ret_val);

    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_number(), 2);
}
*/