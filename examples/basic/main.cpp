#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "mlang/script.hpp"

int main(int argc, char* argv[]) {
    std::ifstream file { "script.txt" };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::Script script { buffer.str() };
    script.execute();

    return 0;
}