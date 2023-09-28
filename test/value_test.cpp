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
    ASSERT_EQ(v2.get_type(), mlang::value_types::number);
    ASSERT_EQ(v3.get_type(), mlang::value_types::number);
    ASSERT_EQ(v4.get_type(), mlang::value_types::boolean);
    ASSERT_EQ(v5.get_type(), mlang::value_types::array);
}