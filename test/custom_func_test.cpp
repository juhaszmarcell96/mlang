#include <gtest/gtest.h>

#include <string>
#include <cmath>

#include "mlang/object/assert.hpp"
#include "mlang/object/number.hpp"
#include "mlang/script/script.hpp"
#include "mlang/func/function.hpp"

class MyFunc : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 2) { throw mlang::RuntimeError{ "MyFunc expects 2 parameters"}; }
        if (params[0].get_typename() != mlang::object::Number::type_name) {
            throw mlang::RuntimeError{ "MyFunc expects the 1st parameter to be of type " + mlang::object::Number::type_name};
        }
        if (params[1].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "MyFunc expects the 2nd parameter to be of type " + mlang::object::String::type_name};
        }
        std::string result;
        for (int i = 0; i < params[0].get_number(); ++i) {
            result += params[1].get_string();
        }
        return mlang::object::Object {std::make_shared<mlang::object::String>(result)};
    }
};


TEST(CustomFuncTest, Test1) {
    std::string script_text;
    script_text += "var a = 3; \n";
    script_text += "var b = \"asd\";\n";
    script_text += "var c = repeat(a, b);";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    MyFunc func {};
    env.declare_function("repeat", &func);
    script.execute(env);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("a").get_number(), 3);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::String::type_name);
    ASSERT_EQ(env.get_variable("b").get_string(), "asd");
    ASSERT_EQ(env.has_variable("c"), true);
    ASSERT_EQ(env.get_variable("c").get_typename(), mlang::object::String::type_name);
    ASSERT_EQ(env.get_variable("c").get_string(), "asdasdasd");
}