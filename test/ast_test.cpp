#include <gtest/gtest.h>

#include <string>

#include "mlang/ast.hpp"

TEST(ASTTest, Test0) {
    mlang::Environment env {};
    mlang::Value ret_val {};
    std::string var_name = "num";

    ASSERT_EQ(env.has_variable(var_name), false);

    mlang::DeclarationOperationNode node {mlang::value_types::number, var_name};
    node.execute(env, ret_val);

    ASSERT_EQ(env.has_variable(var_name), true);
    ASSERT_EQ(env.get_variable(var_name)->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable(var_name)->get_number(), 0);

    std::unique_ptr<mlang::ValueNode> rhs = std::make_unique<mlang::ValueNode>(mlang::Value { 5 });
    mlang::AssignmentOperationNode assignment { var_name, std::move(rhs) };
    assignment.execute(env, ret_val);

    ASSERT_EQ(env.has_variable(var_name), true);
    ASSERT_EQ(env.get_variable(var_name)->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable(var_name)->get_number(), 5);
    ASSERT_EQ(ret_val.get_type(), mlang::value_types::number);
    ASSERT_EQ(ret_val.get_number(), 5);
}