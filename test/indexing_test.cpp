#include <gtest/gtest.h>

#include <string>

#include "mlang/script/script.hpp"

TEST(IndexingTest, Test0) {
    std::string script_text;
    script_text += "var arr = { 1 }; \n";
    script_text += "arr += \"asd\"; \n";
    script_text += "arr += 1000; \n";
    script_text += "arr += 0; \n";
    script_text += "var a = arr[2]; \n";
    script_text += "var b = arr[1]; \n";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    script.execute(env);

    ASSERT_EQ(env.has_variable("arr"), true);
    ASSERT_EQ(env.get_variable("arr").get_typename(), mlang::object::Array::type_name);
    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 1000);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::String::type_name);
    ASSERT_EQ(env.get_variable("b").get_string(), "asd");
}