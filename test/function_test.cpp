#include <gtest/gtest.h>

#include <string>

#include "mlang/script.hpp"

TEST(FunctionTest, Test0) {
    std::string script_text;
    script_text += "function sum (number a, number b) -> number; \n";
    script_text += "    number c = a + b; \n";
    script_text += "    return c; \n";
    script_text += "end; \n";
    script_text += "number num = sum (5, 7); \n";
    mlang::Script script { script_text };
    mlang::EnvStack env {};
    script.execute(env);

    ASSERT_EQ(env.has_variable("num"), true);
    ASSERT_EQ(env.get_variable("num")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("num")->get_number(), 12);
    ASSERT_EQ(env.has_variable("a"), false);
    ASSERT_EQ(env.has_variable("b"), false);
    ASSERT_EQ(env.has_variable("c"), false);
}