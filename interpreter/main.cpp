#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "mlang/script/script.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "ERROR : script path needed as one and only argument" << std::endl;
        return 1;
    }

    std::string script_path { argv[1] };

    std::ifstream file { script_path };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::script::Script script { buffer.str() };
    mlang::script::EnvStack env {};
    script.execute(env);

    return 0;
}