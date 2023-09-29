#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "mlang/tokenizer.hpp"
#include "mlang/token.hpp"
#include "mlang/parser.hpp"
#include "mlang/value.hpp"
#include "mlang/environment.hpp"

TEST(ParserTest, Test0) {
    /* number num; */
    mlang::Token t0 { mlang::token_types::kw_number, 0, 0 };
    mlang::Token t1 { mlang::token_types::identifier, "num", 0, 0 };

    std::vector<mlang::Token*> tokens;
    tokens.push_back(&t0);
    tokens.push_back(&t1);
    mlang::Parser parser { tokens };
    mlang::node_ptr root = parser.parse();

    ASSERT_EQ(root->get_type(), mlang::ast_node_types::declaration);

    mlang::DeclarationOperationNode* node_ptr = dynamic_cast<mlang::DeclarationOperationNode*>(root.get());

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