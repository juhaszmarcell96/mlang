#include <gtest/gtest.h>

#include <string>

#include "mlang/script.hpp"

TEST(WhileLoopTest, Test0) {
    std::string script_text;
    script_text += "number a = 0; \n";
    script_text += "number b = 5; \n";
    script_text += "while (b > 0); \n";
    script_text += "    b -= 1; \n";
    script_text += "    a += 1; \n";
    script_text += "end; \n";
    mlang::Script script { script_text };
    mlang::EnvStack env {};
    script.execute(env);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 5);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("b")->get_number(), 0);
}