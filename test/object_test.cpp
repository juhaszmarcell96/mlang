#include <gtest/gtest.h>

#include <string>

#include "mlang/object/object.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/boolean.hpp"
#include "mlang/object/array.hpp"
#include "mlang/object/none.hpp"
#include "mlang/environment.hpp"

TEST(ObjectTest, Test0) {
    
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

TEST(ObjectTest, Test2) {
    mlang::EnvStack env {};
    env.declare_variable("a", mlang::object::None::type_name);
    mlang::object::Object b { std::make_shared<mlang::object::Number>(3) };

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::None::type_name);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(b.get_number(), 3);

    env.get_variable("a").assign(b);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("a").get_number(), 3);
    ASSERT_EQ(b.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(b.get_number(), 3);
}

TEST(ObjectTest, Test3) {
    mlang::object::Object a {};
    mlang::object::Object b { std::make_shared<mlang::object::Number>(3) };
    mlang::object::Object c { std::make_shared<mlang::object::Number>(5) };
    mlang::object::Object arr { std::make_shared<mlang::object::Array>(std::vector<mlang::object::Object>{a, b, c}) };

    mlang::object::Object one { std::make_shared<mlang::object::Number>(1) };
    mlang::object::Object index { std::make_shared<mlang::object::Number>(0) };

    ASSERT_EQ(arr.get_typename(), mlang::object::Array::type_name);
    ASSERT_EQ(arr.operator_subscript(index).get_typename(), mlang::object::None::type_name);
    index.operator_add_equal(one);
    ASSERT_EQ(arr.operator_subscript(index).get_typename(), mlang::object::Number::type_name);
    index.postfix_increment();
    ASSERT_EQ(arr.operator_subscript(index).get_typename(), mlang::object::Number::type_name);
}

TEST(ObjectTest, Test4) {
    mlang::object::Object a { std::make_shared<mlang::object::Number>(1) };
    mlang::object::Object b { std::make_shared<mlang::object::Number>(3) };
    mlang::object::Object c { std::make_shared<mlang::object::Number>(5) };
    mlang::object::Object arr { std::make_shared<mlang::object::Array>(std::vector<mlang::object::Object>{a, b, c}) };

    mlang::object::Object index { std::make_shared<mlang::object::Number>(0) };

    ASSERT_EQ(arr.get_typename(), mlang::object::Array::type_name);
    ASSERT_EQ(arr.operator_subscript(index).get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(arr.operator_subscript(index).get_number(), 1);
    ASSERT_EQ(arr.operator_subscript(index.postfix_increment()).get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(arr.operator_subscript(index).get_number(), 3);
    ASSERT_EQ(arr.operator_subscript(index).get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(arr.operator_subscript(index).get_number(), 3);
    ASSERT_EQ(arr.operator_subscript(index.prefix_increment()).get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(arr.operator_subscript(index).get_number(), 5);

    arr.operator_subscript(index).postfix_increment();
    ASSERT_EQ(arr.operator_subscript(index).get_number(), 6);
}

TEST(ObjectTest, Test5) {
    mlang::object::Object a { std::make_shared<mlang::object::Number>(1) };

    ASSERT_EQ(a.get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(a.get_number(), 1);
    ASSERT_EQ(a.postfix_increment().get_number(), 1);
    ASSERT_EQ(a.get_number(), 2);
    ASSERT_EQ(a.prefix_increment().get_number(), 3);
    ASSERT_EQ(a.get_number(), 3);
    ASSERT_EQ(a.prefix_decrement().get_number(), 2);
    ASSERT_EQ(a.get_number(), 2);
    ASSERT_EQ(a.postfix_decrement().get_number(), 2);
    ASSERT_EQ(a.get_number(), 1);
}