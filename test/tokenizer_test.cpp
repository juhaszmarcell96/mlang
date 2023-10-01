#include <gtest/gtest.h>

#include <string>

#include "mlang/tokenizer/tokenizer.hpp"

TEST(TokenizerTest, Test0) {
    std::string script = "number a = 5;";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[0].get_value(), "number");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[1].get_value(), "a");
    ASSERT_EQ(tokens[2].get_type(), mlang::tokenizer::token_types::equal_sign);
    ASSERT_EQ(tokens[2].get_value(), "=");
    ASSERT_EQ(tokens[3].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[3].get_value(), "5");
    ASSERT_EQ(tokens[4].get_type(), mlang::tokenizer::token_types::semicolon);
    ASSERT_EQ(tokens[4].get_value(), ";");
}

TEST(TokenizerTest, Test1) {
    std::string script = "1 + (2 - 3 ) * 15 / 3";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 11);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[0].get_value(), "1");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::plus);
    ASSERT_EQ(tokens[1].get_value(), "+");
    ASSERT_EQ(tokens[2].get_type(), mlang::tokenizer::token_types::round_bracket_open);
    ASSERT_EQ(tokens[2].get_value(), "(");
    ASSERT_EQ(tokens[3].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[3].get_value(), "2");
    ASSERT_EQ(tokens[4].get_type(), mlang::tokenizer::token_types::dash);
    ASSERT_EQ(tokens[4].get_value(), "-");
    ASSERT_EQ(tokens[5].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[5].get_value(), "3");
    ASSERT_EQ(tokens[6].get_type(), mlang::tokenizer::token_types::round_bracket_close);
    ASSERT_EQ(tokens[6].get_value(), ")");
    ASSERT_EQ(tokens[7].get_type(), mlang::tokenizer::token_types::asterisk);
    ASSERT_EQ(tokens[7].get_value(), "*");
    ASSERT_EQ(tokens[8].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[8].get_value(), "15");
    ASSERT_EQ(tokens[9].get_type(), mlang::tokenizer::token_types::slash);
    ASSERT_EQ(tokens[9].get_value(), "/");
    ASSERT_EQ(tokens[10].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[10].get_value(), "3");
}

TEST(TokenizerTest, Test2) {
    std::string script = "if (e == false) { return true; }";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 12);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[0].get_value(), "if");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::round_bracket_open);
    ASSERT_EQ(tokens[1].get_value(), "(");
    ASSERT_EQ(tokens[2].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[2].get_value(), "e");
    ASSERT_EQ(tokens[3].get_type(), mlang::tokenizer::token_types::equal_sign);
    ASSERT_EQ(tokens[3].get_value(), "=");
    ASSERT_EQ(tokens[4].get_type(), mlang::tokenizer::token_types::equal_sign);
    ASSERT_EQ(tokens[4].get_value(), "=");
    ASSERT_EQ(tokens[5].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[5].get_value(), "false");
    ASSERT_EQ(tokens[6].get_type(), mlang::tokenizer::token_types::round_bracket_close);
    ASSERT_EQ(tokens[6].get_value(), ")");
    ASSERT_EQ(tokens[7].get_type(), mlang::tokenizer::token_types::curly_bracket_open);
    ASSERT_EQ(tokens[7].get_value(), "{");
    ASSERT_EQ(tokens[8].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[8].get_value(), "return");
    ASSERT_EQ(tokens[9].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[9].get_value(), "true");
    ASSERT_EQ(tokens[10].get_type(), mlang::tokenizer::token_types::semicolon);
    ASSERT_EQ(tokens[10].get_value(), ";");
    ASSERT_EQ(tokens[11].get_type(), mlang::tokenizer::token_types::curly_bracket_close);
    ASSERT_EQ(tokens[11].get_value(), "}");
}

TEST(TokenizerTest, Test3) {
    std::string script = "# asd !@# >\n < \" 12.4 - o \" 2.75\n\r ? ~";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 11);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::hashtag);
    ASSERT_EQ(tokens[0].get_value(), "#");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[1].get_value(), "asd");
    ASSERT_EQ(tokens[2].get_type(), mlang::tokenizer::token_types::exclamation_mark);
    ASSERT_EQ(tokens[2].get_value(), "!");
    ASSERT_EQ(tokens[3].get_type(), mlang::tokenizer::token_types::at);
    ASSERT_EQ(tokens[3].get_value(), "@");
    ASSERT_EQ(tokens[4].get_type(), mlang::tokenizer::token_types::hashtag);
    ASSERT_EQ(tokens[4].get_value(), "#");
    ASSERT_EQ(tokens[5].get_type(), mlang::tokenizer::token_types::greater);
    ASSERT_EQ(tokens[5].get_value(), ">");
    ASSERT_EQ(tokens[6].get_type(), mlang::tokenizer::token_types::less);
    ASSERT_EQ(tokens[6].get_value(), "<");
    ASSERT_EQ(tokens[7].get_type(), mlang::tokenizer::token_types::string);
    ASSERT_EQ(tokens[7].get_value(), " 12.4 - o ");
    ASSERT_EQ(tokens[8].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[8].get_value(), "2.75");
    ASSERT_EQ(tokens[9].get_type(), mlang::tokenizer::token_types::question_mark);
    ASSERT_EQ(tokens[9].get_value(), "?");
    ASSERT_EQ(tokens[10].get_type(), mlang::tokenizer::token_types::tilde);
    ASSERT_EQ(tokens[10].get_value(), "~");
}

TEST(TokenizerTest, Test4) {
    std::string script = "13.65.3";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[0].get_value(), "13.65");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[1].get_value(), "0.3");
}

TEST(TokenizerTest, Test5) {
    std::string script = ".15f - asd % \t ; $";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 7);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::number);
    ASSERT_EQ(tokens[0].get_value(), "0.15");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[1].get_value(), "f");
    ASSERT_EQ(tokens[2].get_type(), mlang::tokenizer::token_types::dash);
    ASSERT_EQ(tokens[2].get_value(), "-");
    ASSERT_EQ(tokens[3].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[3].get_value(), "asd");
    ASSERT_EQ(tokens[4].get_type(), mlang::tokenizer::token_types::percent);
    ASSERT_EQ(tokens[4].get_value(), "%");
    ASSERT_EQ(tokens[5].get_type(), mlang::tokenizer::token_types::semicolon);
    ASSERT_EQ(tokens[5].get_value(), ";");
    ASSERT_EQ(tokens[6].get_type(), mlang::tokenizer::token_types::dollar);
    ASSERT_EQ(tokens[6].get_value(), "$");
}

TEST(TokenizerTest, Test6) {
    std::string script = "bool get_val () { return true; }";
    mlang::tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();
    auto tokens = tokenizer.get_tokens();

    ASSERT_EQ(tokens.size(), 9);
    ASSERT_EQ(tokens[0].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[0].get_value(), "bool");
    ASSERT_EQ(tokens[1].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[1].get_value(), "get_val");
    ASSERT_EQ(tokens[2].get_type(), mlang::tokenizer::token_types::round_bracket_open);
    ASSERT_EQ(tokens[2].get_value(), "(");
    ASSERT_EQ(tokens[3].get_type(), mlang::tokenizer::token_types::round_bracket_close);
    ASSERT_EQ(tokens[3].get_value(), ")");
    ASSERT_EQ(tokens[4].get_type(), mlang::tokenizer::token_types::curly_bracket_open);
    ASSERT_EQ(tokens[4].get_value(), "{");
    ASSERT_EQ(tokens[5].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[5].get_value(), "return");
    ASSERT_EQ(tokens[6].get_type(), mlang::tokenizer::token_types::identifier);
    ASSERT_EQ(tokens[6].get_value(), "true");
    ASSERT_EQ(tokens[7].get_type(), mlang::tokenizer::token_types::semicolon);
    ASSERT_EQ(tokens[7].get_value(), ";");
    ASSERT_EQ(tokens[8].get_type(), mlang::tokenizer::token_types::curly_bracket_close);
    ASSERT_EQ(tokens[8].get_value(), "}");
}