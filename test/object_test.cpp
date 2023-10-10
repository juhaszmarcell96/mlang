#include <gtest/gtest.h>

#include <string>

#include "mlang/object/object.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/boolean.hpp"
#include "mlang/object/none.hpp"
#include "mlang/environment.hpp"

TEST(ObjectTest, Test0) {
    mlang::EnvStack env {};
    
    mlang::object::Object a {};
    mlang::object::Object b { mlang::Environment::get_factory(mlang::object::Number::type_name) };
    mlang::object::Object c { std::make_shared<mlang::object::Number>(5) };

    ASSERT_EQ(a.get_typename(), mlang::object::None::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(b.get_number(), 0);
    ASSERT_EQ(c.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(c.get_number(), 5);

    a.assign(c);

    ASSERT_EQ(a.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(c.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(a.get_number(), 5);
    ASSERT_EQ(b.get_number(), 0);
    ASSERT_EQ(c.get_number(), 5);
}

TEST(ObjectTest, Test1) {
    mlang::EnvStack env {};
    
    mlang::object::Object a {};
    mlang::object::Object b { std::make_shared<mlang::object::Number>(3) };
    mlang::object::Object c { std::make_shared<mlang::object::Number>(5) };

    ASSERT_EQ(a.get_typename(), mlang::object::None::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(b.get_number(), 3);
    ASSERT_EQ(c.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(c.get_number(), 5);

    a.assign(c.operator_binary_add(b));

    ASSERT_EQ(a.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(c.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(a.get_number(), 8);
    ASSERT_EQ(b.get_number(), 3);
    ASSERT_EQ(c.get_number(), 5);

    a.assign(c.operator_comparison_equal(b));

    ASSERT_EQ(a.get_typename(), mlang::object::Boolean::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(c.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(a.is_true(), false);
    ASSERT_EQ(b.get_number(), 3);
    ASSERT_EQ(c.get_number(), 5);

    a.assign(c.operator_comparison_not_equal(b));

    ASSERT_EQ(a.get_typename(), mlang::object::Boolean::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(c.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(a.is_true(), true);
    ASSERT_EQ(b.get_number(), 3);
    ASSERT_EQ(c.get_number(), 5);
}