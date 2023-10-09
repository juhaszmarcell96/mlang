#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "mlang/script.hpp"
#include "mlang/environment.hpp"

int main(int argc, char* argv[]) {
    std::string executable_path { argv[0] };
    std::filesystem::path p { executable_path };
    p.replace_filename("script.txt");

    std::ifstream file { p };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::Script script { buffer.str() };
    mlang::EnvStack env {};
    script.execute(env);

    return 0;
}