#include <gtest/gtest.h>

#include <string>

#include "mlang/script/script.hpp"

TEST(ScriptTest, Test0) {
    std::string script_text = "var a = 5; var b = 5.1;";
    mlang::script::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 10);
    ASSERT_EQ(tokens[0].type, mlang::script::token_types::kw_var);
    ASSERT_EQ(tokens[1].type, mlang::script::token_types::identifier);
    ASSERT_EQ(tokens[1].value_str, "a");
    ASSERT_EQ(tokens[2].type, mlang::script::token_types::equal_sign);
    ASSERT_EQ(tokens[3].type, mlang::script::token_types::integer);
    ASSERT_EQ(tokens[3].value_int, 5);
    ASSERT_EQ(tokens[4].type, mlang::script::token_types::semicolon);
    ASSERT_EQ(tokens[5].type, mlang::script::token_types::kw_var);
    ASSERT_EQ(tokens[6].type, mlang::script::token_types::identifier);
    ASSERT_EQ(tokens[6].value_str, "b");
    ASSERT_EQ(tokens[7].type, mlang::script::token_types::equal_sign);
    ASSERT_EQ(tokens[8].type, mlang::script::token_types::floating);
    ASSERT_EQ(tokens[8].value_float, 5.1);
    ASSERT_EQ(tokens[9].type, mlang::script::token_types::semicolon);
}

TEST(ScriptTest, Test1) {
    std::string script_text = "if (e == false) { return true; }";
    mlang::script::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 11);
    ASSERT_EQ(tokens[0].type, mlang::script::token_types::kw_if);
    ASSERT_EQ(tokens[1].type, mlang::script::token_types::round_bracket_open);
    ASSERT_EQ(tokens[2].type, mlang::script::token_types::identifier);
    ASSERT_EQ(tokens[2].value_str, "e");
    ASSERT_EQ(tokens[3].type, mlang::script::token_types::double_equal);
    ASSERT_EQ(tokens[4].type, mlang::script::token_types::kw_false);
    ASSERT_EQ(tokens[5].type, mlang::script::token_types::round_bracket_close);
    ASSERT_EQ(tokens[6].type, mlang::script::token_types::curly_bracket_open);
    ASSERT_EQ(tokens[7].type, mlang::script::token_types::kw_return);
    ASSERT_EQ(tokens[8].type, mlang::script::token_types::kw_true);
    ASSERT_EQ(tokens[9].type, mlang::script::token_types::semicolon);
    ASSERT_EQ(tokens[10].type, mlang::script::token_types::curly_bracket_close);
}

TEST(ScriptTest, Test2) {
    std::string script_text = "var str = \"asd\";\nstr += \"fgh\";";
    mlang::script::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 9);
    ASSERT_EQ(tokens[0].type, mlang::script::token_types::kw_var);
    ASSERT_EQ(tokens[1].type, mlang::script::token_types::identifier);
    ASSERT_EQ(tokens[1].value_str, "str");
    ASSERT_EQ(tokens[2].type, mlang::script::token_types::equal_sign);
    ASSERT_EQ(tokens[3].type, mlang::script::token_types::string);
    ASSERT_EQ(tokens[3].value_str, "asd");
    ASSERT_EQ(tokens[4].type, mlang::script::token_types::semicolon);
    ASSERT_EQ(tokens[5].type, mlang::script::token_types::identifier);
    ASSERT_EQ(tokens[5].value_str, "str");
    ASSERT_EQ(tokens[6].type, mlang::script::token_types::plus_equal);
    ASSERT_EQ(tokens[7].type, mlang::script::token_types::string);
    ASSERT_EQ(tokens[7].value_str, "fgh");
    ASSERT_EQ(tokens[8].type, mlang::script::token_types::semicolon);
}

TEST(ScriptTest, Test3) {
    std::string script_text = "+++-=//=!=<>=";
    mlang::script::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 8);
    ASSERT_EQ(tokens[0].type, mlang::script::token_types::plus_plus);
    ASSERT_EQ(tokens[1].type, mlang::script::token_types::plus);
    ASSERT_EQ(tokens[2].type, mlang::script::token_types::dash_equal);
    ASSERT_EQ(tokens[3].type, mlang::script::token_types::slash);
    ASSERT_EQ(tokens[4].type, mlang::script::token_types::slash_equal);
    ASSERT_EQ(tokens[5].type, mlang::script::token_types::exclamation_equal);
    ASSERT_EQ(tokens[6].type, mlang::script::token_types::less);
    ASSERT_EQ(tokens[7].type, mlang::script::token_types::greater_equal);
}

TEST(ScriptTest, Test4) {
    std::string script_text = "var m_state = false;";
    mlang::script::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens[0].type, mlang::script::token_types::kw_var);
    ASSERT_EQ(tokens[1].type, mlang::script::token_types::identifier);
    ASSERT_EQ(tokens[1].value_str, "m_state");
    ASSERT_EQ(tokens[2].type, mlang::script::token_types::equal_sign);
    ASSERT_EQ(tokens[3].type, mlang::script::token_types::kw_false);
    ASSERT_EQ(tokens[4].type, mlang::script::token_types::semicolon);
}

TEST(ScriptTest, Test5) {
    std::string script_text = "var num;";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("num"), true);
    ASSERT_EQ(env.get_variable("num").get_typename(), mlang::object::None::type_name);
}

TEST(ScriptTest, Test6) {
    std::string script_text = "var n; var s; var b; var a;";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);

    ASSERT_EQ(env.has_variable("n"), true);
    ASSERT_EQ(env.get_variable("n").get_typename(), mlang::object::None::type_name);
    ASSERT_EQ(env.has_variable("s"), true);
    ASSERT_EQ(env.get_variable("s").get_typename(), mlang::object::None::type_name);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::None::type_name);
    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::None::type_name);
}

TEST(ScriptTest, Test7) {
    std::string script_text = "var n; \n";
    script_text += "n = 5;";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("n"), true);
    ASSERT_EQ(env.get_variable("n").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("n").get_int(), 5);
}

TEST(ScriptTest, Test8) {
    std::string script_text;
    script_text += "var a; \n";
    script_text += "var b; \n";
    script_text += "var c; \n";
    script_text += "a = 5; \n";
    script_text += "b = a + 2; \n";
    script_text += "c = 3 * ( a + 2 ) - ( b - 2 );";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 5);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("b").get_int(), 7);
    ASSERT_EQ(env.has_variable("c"), true);
    ASSERT_EQ(env.get_variable("c").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("c").get_int(), 16);

    script_text.clear();
    script_text += "a -= 3; \n";
    script_text += "b *= a; \n";
    script_text += "c /= b * 2 - 10 * a;";
    mlang::script::Script script2 { script_text };
    ret = script2.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 2);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("b").get_int(), 14);
    ASSERT_EQ(env.has_variable("c"), true);
    ASSERT_EQ(env.get_variable("c").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("c").get_int(), 2);
}

TEST(ScriptTest, Test9) {
    std::string script_text;
    script_text += "var a; \n";
    script_text += "var b; \n";
    script_text += "var c; \n";
    script_text += "a = 5; \n";
    script_text += "if (a == 5) { \n";
    script_text += "    b = 10; \n";
    script_text += "} \n";
    script_text += "if (a == 4) { \n";
    script_text += "    c = 3; \n";
    script_text += "}";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 5);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("b").get_int(), 10);
    ASSERT_EQ(env.has_variable("c"), true);
    ASSERT_EQ(env.get_variable("c").get_typename(), mlang::object::None::type_name);
}

TEST(ScriptTest, Test10) {
    std::string script_text;
    script_text += "var a; \n";
    script_text += "var b; \n";
    script_text += "var c; \n";
    script_text += "var d; \n";
    script_text += "a = 5; \n";
    script_text += "if (a == 5) { b = 10; }\n";
    script_text += "else { b = 11; } \n";
    script_text += "\n";
    script_text += "if (a > 5) { \n";
    script_text += "    c = 3; \n";
    script_text += "} else if (a == 5) { \n";
    script_text += "    c = 4; \n";
    script_text += "} \n";
    script_text += "\n";
    script_text += "if (a > 5) { \n";
    script_text += "    d = 100; \n";
    script_text += "} else if (a < 5) { \n";
    script_text += "    d = 101; \n";
    script_text += "} else { \n";
    script_text += "    d = 102; \n";
    script_text += "} \n";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 5);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("b").get_int(), 10);
    ASSERT_EQ(env.has_variable("c"), true);
    ASSERT_EQ(env.get_variable("c").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("c").get_int(), 4);
    ASSERT_EQ(env.has_variable("d"), true);
    ASSERT_EQ(env.get_variable("d").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("d").get_int(), 102);
}

TEST(ScriptTest, Test11) {
    std::string script_text;
    script_text += "var a = 5; \n";
    script_text += "var b = a + 2; \n";
    script_text += "var c = 3 * ( a + 2 ) - ( b - 2 );";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 5);
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("b").get_int(), 7);
    ASSERT_EQ(env.has_variable("c"), true);
    ASSERT_EQ(env.get_variable("c").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("c").get_int(), 16);
}

TEST(ScriptTest, Test12) {
    std::string script_text;
    script_text += "var a = 5; \n";
    script_text += "var i; \n";
    script_text += "for (i = 0; i < 10; i += 1){ \n";
    script_text += "    a += 2; \n";
    script_text += "}";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 25);
    ASSERT_EQ(env.has_variable("i"), true);
    ASSERT_EQ(env.get_variable("i").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("i").get_int(), 10);
}

TEST(ScriptTest, Test13) {
    std::string script_text;
    script_text += "var a = 5; \n";
    script_text += "if (a == 5) { \n";
    script_text += "    var b = 2;\n";
    script_text += "}";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    int ret = script.execute(env);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), mlang::object::Int::type_name);
    ASSERT_EQ(env.get_variable("a").get_int(), 5);
    ASSERT_EQ(env.has_variable("b"), false);
}