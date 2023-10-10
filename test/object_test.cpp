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
    
    mlang::Object a {};
    mlang::Object b { mlang::Environment::get_factory(mlang::Number::type_name) };

    ASSERT_EQ(a.get_typename(), mlang::None::type_name);
    ASSERT_EQ(b.get_typename(), mlang::Number::type_name);

    a.assign()
}