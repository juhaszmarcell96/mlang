#include <gtest/gtest.h>

#include <string>

#include "mlang/tokenizer.hpp"
#include "mlang/script.hpp"
#include "mlang/environment.hpp"

TEST(ScriptTest, Test0) {
    std::string script_text = "number a = 5;";
    mlang::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens[0].type, mlang::token_types::kw_number);
    ASSERT_EQ(tokens[1].type, mlang::token_types::identifier);
    ASSERT_EQ(tokens[1].value_str, "a");
    ASSERT_EQ(tokens[2].type, mlang::token_types::equal_sign);
    ASSERT_EQ(tokens[3].type, mlang::token_types::number);
    ASSERT_EQ(tokens[3].value_num, 5);
    ASSERT_EQ(tokens[4].type, mlang::token_types::semicolon);
}

TEST(ScriptTest, Test1) {
    std::string script_text = "if (e == false) { return true; }";
    mlang::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 11);
    ASSERT_EQ(tokens[0].type, mlang::token_types::kw_if);
    ASSERT_EQ(tokens[1].type, mlang::token_types::round_bracket_open);
    ASSERT_EQ(tokens[2].type, mlang::token_types::identifier);
    ASSERT_EQ(tokens[2].value_str, "e");
    ASSERT_EQ(tokens[3].type, mlang::token_types::double_equal);
    ASSERT_EQ(tokens[4].type, mlang::token_types::kw_false);
    ASSERT_EQ(tokens[5].type, mlang::token_types::round_bracket_close);
    ASSERT_EQ(tokens[6].type, mlang::token_types::curly_bracket_open);
    ASSERT_EQ(tokens[7].type, mlang::token_types::kw_return);
    ASSERT_EQ(tokens[8].type, mlang::token_types::kw_true);
    ASSERT_EQ(tokens[9].type, mlang::token_types::semicolon);
    ASSERT_EQ(tokens[10].type, mlang::token_types::curly_bracket_close);
}

TEST(ScriptTest, Test2) {
    std::string script_text = "string str = \"asd\";\nstr += \"fgh\";";
    mlang::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 9);
    ASSERT_EQ(tokens[0].type, mlang::token_types::kw_string);
    ASSERT_EQ(tokens[1].type, mlang::token_types::identifier);
    ASSERT_EQ(tokens[1].value_str, "str");
    ASSERT_EQ(tokens[2].type, mlang::token_types::equal_sign);
    ASSERT_EQ(tokens[3].type, mlang::token_types::string);
    ASSERT_EQ(tokens[3].value_str, "asd");
    ASSERT_EQ(tokens[4].type, mlang::token_types::semicolon);
    ASSERT_EQ(tokens[5].type, mlang::token_types::identifier);
    ASSERT_EQ(tokens[5].value_str, "str");
    ASSERT_EQ(tokens[6].type, mlang::token_types::plus_equal);
    ASSERT_EQ(tokens[7].type, mlang::token_types::string);
    ASSERT_EQ(tokens[7].value_str, "fgh");
    ASSERT_EQ(tokens[8].type, mlang::token_types::semicolon);
}

TEST(ScriptTest, Test3) {
    std::string script_text = "+++-=//=!=<>=";
    mlang::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 8);
    ASSERT_EQ(tokens[0].type, mlang::token_types::plus_plus);
    ASSERT_EQ(tokens[1].type, mlang::token_types::plus);
    ASSERT_EQ(tokens[2].type, mlang::token_types::dash_equal);
    ASSERT_EQ(tokens[3].type, mlang::token_types::slash);
    ASSERT_EQ(tokens[4].type, mlang::token_types::slash_equal);
    ASSERT_EQ(tokens[5].type, mlang::token_types::exclamation_equal);
    ASSERT_EQ(tokens[6].type, mlang::token_types::less);
    ASSERT_EQ(tokens[7].type, mlang::token_types::greater_equal);
}

TEST(ScriptTest, Test4) {
    std::string script_text = "bool m_state = false;";
    mlang::Script script { script_text };
    auto tokens = script.get_tokens();

    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens[0].type, mlang::token_types::kw_bool);
    ASSERT_EQ(tokens[1].type, mlang::token_types::identifier);
    ASSERT_EQ(tokens[1].value_str, "m_state");
    ASSERT_EQ(tokens[2].type, mlang::token_types::equal_sign);
    ASSERT_EQ(tokens[3].type, mlang::token_types::kw_false);
    ASSERT_EQ(tokens[4].type, mlang::token_types::semicolon);
}

//TEST(ScriptTest, Test5) {
//    std::string script_text = "number num;";
//    mlang::Script script { script_text };
//    mlang::Environment env {};
//    script.execute(env);
//
//    ASSERT_EQ(env.has_variable("num"), true);
//}