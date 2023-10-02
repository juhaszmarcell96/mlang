#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "mlang/tokenizer/tokenizer.hpp"
#include "mlang/token.hpp"
#include "mlang/parser/parser.hpp"
#include "mlang/value.hpp"
#include "mlang/environment.hpp"

TEST(ParserTest, Test0) {
    /* number num; */
    mlang::Token t0 { mlang::token_types::kw_number, 0, 0 };
    mlang::Token t1 { mlang::token_types::identifier, "num", 0, 0 };
    mlang::Token t2 { mlang::token_types::semicolon, 0, 0 };

    std::vector<mlang::Token> tokens;
    tokens.push_back(t0);
    tokens.push_back(t1);
    tokens.push_back(t2);
    mlang::Parser parser {};
    mlang::node_ptr root = parser.parse(tokens);

    ASSERT_EQ(root->get_type(), mlang::ast_node_types::main);
    mlang::MainNode* main_ptr = dynamic_cast<mlang::MainNode*>(root.get());
    ASSERT_EQ(main_ptr->get_nodes().size(), 1);

    ASSERT_EQ(main_ptr->get_nodes()[0]->get_type(), mlang::ast_node_types::declaration);

    mlang::DeclarationOperationNode* node_ptr = dynamic_cast<mlang::DeclarationOperationNode*>(main_ptr->get_nodes()[0].get());

    ASSERT_NE(node_ptr, nullptr);
    ASSERT_EQ(node_ptr->get_var_type(), mlang::value_types::number);
    ASSERT_EQ(node_ptr->get_var_name(), "num");

    mlang::Value ret_val {};
    mlang::Environment env {};

    ASSERT_EQ(env.has_variable("num"), false);

    root->execute(env, ret_val);

    ASSERT_EQ(env.has_variable("num"), true);
    ASSERT_EQ(env.get_variable("num")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("num")->get_number(), 0);
}

TEST(ParserTest, Test1) {
    /* num = 5; */
    mlang::Value ret_val {};
    mlang::Environment env {};
    env.declare_variable("num", mlang::value_types::number);

    mlang::Token t0 { mlang::token_types::identifier, "num", 0, 0 };
    mlang::Token t1 { mlang::token_types::equal_sign, 0, 0 };
    mlang::Token t2 { mlang::token_types::number, 5, 0, 0 };
    mlang::Token t3 { mlang::token_types::semicolon, 0, 0 };

    std::vector<mlang::Token> tokens;
    tokens.push_back(t0);
    tokens.push_back(t1);
    tokens.push_back(t2);
    tokens.push_back(t3);
    mlang::Parser parser { };
    mlang::node_ptr root = parser.parse(tokens);

    ASSERT_EQ(root->get_type(), mlang::ast_node_types::main);
    mlang::MainNode* main_ptr = dynamic_cast<mlang::MainNode*>(root.get());
    ASSERT_EQ(main_ptr->get_nodes().size(), 1);

    ASSERT_EQ(main_ptr->get_nodes()[0]->get_type(), mlang::ast_node_types::assignment);

    mlang::AssignmentOperationNode* node_ptr = dynamic_cast<mlang::AssignmentOperationNode*>(main_ptr->get_nodes()[0].get());

    ASSERT_NE(node_ptr, nullptr);
    ASSERT_EQ(node_ptr->get_var_name(), "num");
    ASSERT_NE(node_ptr->get_right(), nullptr);
    ASSERT_EQ(node_ptr->get_right()->get_type(), mlang::ast_node_types::value);

    ASSERT_EQ(env.has_variable("num"), true);
    ASSERT_EQ(env.get_variable("num")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("num")->get_number(), 0);

    root->execute(env, ret_val);

    ASSERT_EQ(env.has_variable("num"), true);
    ASSERT_EQ(env.get_variable("num")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("num")->get_number(), 5);
}