#include <gtest/gtest.h>

#include <string>

#include "mlang/tokenizer/tokenizer.hpp"
#include "mlang/script.hpp"
#include "mlang/environment.hpp"
#include "mlang/exception.hpp"

TEST(ErrorTest, Test0) {
    std::string script_text;
    script_text += "number a = 5; \n";
    script_text += "number i = a"; // missing ';'
    mlang::Script script { script_text };
    mlang::EnvStack env {};

    try {
        script.execute(env);
        FAIL();
    }
    catch (const mlang::LangException& e) {
        //std::cout << e.what() << std::endl;
        SUCCEED();
    }
}

TEST(ErrorTest, Test1) {
    std::string script_text;
    script_text += "string a; \n";
    script_text += "number i = a;"; // incompatible types
    mlang::Script script { script_text };
    mlang::EnvStack env {};

    try {
        script.execute(env);
        FAIL();
    }
    catch (const mlang::LangException& e) {
        //std::cout << e.what() << std::endl;
        SUCCEED();
    }
}

TEST(ErrorTest, Test2) {
    std::string script_text;
    script_text += "number a; \n";
    script_text += "a = \"hmm\";"; // incompatible types
    mlang::Script script { script_text };
    mlang::EnvStack env {};

    try {
        script.execute(env);
        FAIL();
    }
    catch (const mlang::LangException& e) {
        //std::cout << e.what() << std::endl;
        SUCCEED();
    }
}

TEST(ErrorTest, Test3) {
    std::string script_text;
    script_text += "string a = 5;"; // incompatible types
    mlang::Script script { script_text };
    mlang::EnvStack env {};

    try {
        script.execute(env);
        FAIL();
    }
    catch (const mlang::LangException& e) {
        //std::cout << e.what() << std::endl;
        SUCCEED();
    }
}

TEST(ErrorTest, Test4) {
    std::string script_text;
    script_text += "number a = 5 \n"; // missing ';'
    script_text += "string b; \n";
    script_text += "b = \"hmm\";";
    mlang::Script script { script_text };
    mlang::EnvStack env {};

    try {
        script.execute(env);
        FAIL();
    }
    catch (const mlang::LangException& e) {
        //std::cout << e.what() << std::endl;
        SUCCEED();
    }
}