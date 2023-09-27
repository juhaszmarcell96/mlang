#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "mlang/value.hpp"
#include "mlang/script.hpp"

int main(int argc, char* argv) {
    lang::Value v1 { "asd" };
    lang::Value v2 { 2 };

    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;

    std::cout << v2++ << std::endl;
    std::cout << ++v2 << std::endl;

    lang::Value arr { lang::value_types::vector };
    arr.append(v1);
    arr.append(v2);
    std::cout << arr << std::endl;

    std::ifstream file { "script.txt" };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    lang::Script script { buffer.str() };
    script.execute();

	return 0;
}