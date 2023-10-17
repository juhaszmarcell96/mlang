#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "mlang/script/script.hpp"
#include "mlang/script/environment.hpp"

int main(int argc, char* argv[]) {
    std::string executable_path { argv[0] };
    std::filesystem::path p { executable_path };
    p.replace_filename("script.mlang");

    std::ifstream file { p };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::script::Script script { buffer.str() };
    mlang::script::EnvStack env {};
    script.execute(env);

    return 0;
}