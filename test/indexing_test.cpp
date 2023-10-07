#include <gtest/gtest.h>

#include <string>

#include "mlang/script.hpp"

TEST(IndexingTest, Test0) {
    std::string script_text;
    script_text += "array arr; \n";
    script_text += "arr += 1; \n";
    script_text += "arr += \"asd\"; \n";
    script_text += "arr += 1000; \n";
    script_text += "arr += 0; \n";
    script_text += "number a = arr[2]; \n";
    script_text += "string b = arr[1]; \n";
    mlang::Script script { script_text };
    mlang::EnvStack env {};
    script.execute(env);

    ASSERT_EQ(env.has_variable("arr"), true);
    ASSERT_EQ(env.get_variable("arr")->get_type(), mlang::value_types::array);
    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a")->get_type(), mlang::value_types::number);
    ASSERT_EQ(env.get_variable("a")->get_number(), 1000);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b")->get_type(), mlang::value_types::string);
    ASSERT_EQ(env.get_variable("b")->get_string(), "asd");
}