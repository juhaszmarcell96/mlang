#include <gtest/gtest.h>

#include <string>

#include "mlang/value.hpp"

TEST(ValueTest, Test0) {
    mlang::Value v1 { "asd" };
    mlang::Value v2 { 2 };
    mlang::Value v3 { 1.25 };
    mlang::Value v4 { false };
    mlang::Value v5 { mlang::value_types::array };
    //arr.append(v1);
    //arr.append(v2);

    ASSERT_EQ(v1.get_type(), mlang::value_types::string);
    ASSERT_EQ(v1.get_string(), "asd");
    ASSERT_EQ(v2.get_type(), mlang::value_types::number);
    ASSERT_EQ(v2.get_number(), 2);
    ASSERT_EQ(v3.get_type(), mlang::value_types::number);
    ASSERT_EQ(v3.get_number(), 1.25);
    ASSERT_EQ(v4.get_type(), mlang::value_types::boolean);
    ASSERT_EQ(v4.get_boolean(), false);
    ASSERT_EQ(v5.get_type(), mlang::value_types::array);
}

TEST(ValueTest, Test1) {
    mlang::Value v1 { 2 };

    v1 = v1 + mlang::Value { 3 };
    ASSERT_EQ(v1.get_number(), 5);
    v1 = v1 / mlang::Value { 10 };
    ASSERT_EQ(v1.get_number(), 0.5);
    v1 = v1 * mlang::Value { 4 };
    ASSERT_EQ(v1.get_number(), 2);
    v1 = v1 - mlang::Value { 6 };
    ASSERT_EQ(v1.get_number(), -4);
}

TEST(ValueTest, Test2) {
    mlang::Value v1 { 2 };

    ASSERT_EQ(--v1, mlang::Value { 1 });
    ASSERT_EQ(v1  , mlang::Value { 1 });
    ASSERT_EQ(v1--, mlang::Value { 1 });
    ASSERT_EQ(v1  , mlang::Value { 0 });
    ASSERT_EQ(++v1, mlang::Value { 1 });
    ASSERT_EQ(v1  , mlang::Value { 1 });
    ASSERT_EQ(v1++, mlang::Value { 1 });
    ASSERT_EQ(v1  , mlang::Value { 2 });
}