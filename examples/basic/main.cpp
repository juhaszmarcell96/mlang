#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "mlang/script.hpp"
#include "mlang/environment.hpp"

int main(int argc, char* argv[]) {
    /*
    std::ifstream file { "script.txt" };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::Script script { buffer.str() };
    mlang::Environment env {};
    script.execute(env);
    */

    std::string script_text = "number num;";
    mlang::Script script { script_text };
    mlang::Environment env {};
    script.execute(env);

    return 0;
}