#include <gtest/gtest.h>

#include <string>

#include "mlang/script.hpp"

TEST(WhileLoopTest, Test0) {
    std::string script_text;
    script_text += "var a = 0; \n";
    script_text += "var b = 5; \n";
    script_text += "while (b > 0) { \n";
    script_text += "    b -= 1; \n";
    script_text += "    a += 1; \n";
    script_text += "} \n";
    mlang::Script script { script_text };
    mlang::EnvStack env {};
    script.execute(env);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("a").get_number(), 5);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("b").get_number(), 0);
}

TEST(WhileLoopTest, Test1) {
    std::string script_text;
    script_text += "var a = 0; \n";
    script_text += "while (true) { \n";
    script_text += "    a += 1; \n";
    script_text += "    if (a == 100) { \n";
    script_text += "        break; \n";
    script_text += "    } \n";
    script_text += "} \n";
    mlang::Script script { script_text };
    mlang::EnvStack env {};
    script.execute(env);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("a").get_number(), 100);
}