#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <chrono>

#include "mlang/object/number.hpp"
#include "mlang/script/script.hpp"
#include "mlang/func/function.hpp"
#include "mlang/script/environment.hpp"

class FuncGetTime : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        std::string time_str(30, '\0');
        std::strftime(&time_str[0], time_str.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&t_c));
        return mlang::object::Object {std::make_shared<mlang::object::String>(time_str)};
    }
};

int main(int argc, char* argv[]) {
    std::string executable_path { argv[0] };
    std::filesystem::path p { executable_path };
    p.replace_filename("script.txt");

    std::ifstream file { p };
    if (!file.is_open()) return 1;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::script::Script script { buffer.str() };
    mlang::script::EnvStack env {};
    FuncGetTime func {};
    env.declare_function("get_time", &func);
    script.execute(env);

    return 0;
}