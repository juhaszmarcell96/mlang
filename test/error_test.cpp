#include <gtest/gtest.h>

#include <string>

#include "mlang/tokenizer/tokenizer.hpp"
#include "mlang/script/script.hpp"
#include "mlang/script/environment.hpp"
#include "mlang/exception.hpp"

TEST(ErrorTest, Test0) {
    std::string script_text;
    script_text += "var a = 5; \n";
    script_text += "var i = a"; // missing ';'
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};

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
    script_text += "number a = 5 \n"; // missing ';'
    script_text += "string b; \n";
    script_text += "b = \"hmm\";";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};

    try {
        script.execute(env);
        FAIL();
    }
    catch (const mlang::LangException& e) {
        //std::cout << e.what() << std::endl;
        SUCCEED();
    }
}