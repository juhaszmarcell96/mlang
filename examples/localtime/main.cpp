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

/**
 *  parameters:
 *     - path (String)
 **/
class FuncReadFile : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 1) { throw mlang::RuntimeError{ "FuncReadFile expects 1 parameters"}; }
        if (params[0].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncReadFile expects the 1st parameter to be of type " + mlang::object::String::type_name};
        }
        std::string path { params[0].get_string() };
        std::ifstream file { path };
        if (!file.is_open()) {
            throw mlang::RuntimeError{ "FuncReadFile could not open file " + path };
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return mlang::object::Object {std::make_shared<mlang::object::String>(buffer.str())};
    }
};

/**
 *  parameters:
 *     - id (String)
 *     - instance (Number (integer))
 *     - value (String)
 **/
class FuncSetParameter : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 3) { throw mlang::RuntimeError{ "FuncSetParameter expects 3 parameters"}; }
        if (params[0].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncSetParameter expects the 1st parameter to be of type " + mlang::object::String::type_name};
        }
        if (params[1].get_typename() != mlang::object::Number::type_name) {
            throw mlang::RuntimeError{ "FuncSetParameter expects the 2nd parameter to be of type " + mlang::object::Number::type_name};
        }
        if (params[2].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncSetParameter expects the 3rd parameter to be of type " + mlang::object::String::type_name};
        }

        std::cout << "Setting parameter '" << params[0].get_string() << "' instance " << (int)params[1].get_number() << " to '" << params[2].get_string() << "'" << std::endl;

        return mlang::object::Object {};
    }
};

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
    FuncReadFile fileReadFunc {};
    FuncSetParameter setParamFunc {};
    env.declare_function("read_file", &fileReadFunc);
    env.declare_function("set_parameter", &setParamFunc);
    script.execute(env);

    return 0;
}