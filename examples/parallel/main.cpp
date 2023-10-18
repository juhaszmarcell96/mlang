#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <thread>

#include "mlang/object/number.hpp"
#include "mlang/script/script.hpp"
#include "mlang/func/function.hpp"
#include "mlang/script/environment.hpp"

#include "helper_functions.hpp"

std::string executable_path;

FuncObserverRegister observerRegisterFunc {};
FuncNotificationWait notificationWaitFunc {};
FuncSetParameter setParamFunc {};
FuncSleep sleepFunc {};

void script_1 () {
    std::filesystem::path path { executable_path };
    path.replace_filename("script_1.mlang");

    std::ifstream file { path };
    if (!file.is_open()) exit(1);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::script::EnvStack env {};

    env.declare_function("register_observer", &observerRegisterFunc);
    env.declare_function("wait_for_notification", &notificationWaitFunc);
    env.declare_function("set_parameter", &setParamFunc);
    env.declare_function("sleep", &sleepFunc);

    mlang::script::Script script { buffer.str() };
    script.execute(env);
}

void script_2 () {
    std::filesystem::path path { executable_path };
    path.replace_filename("script_2.mlang");

    std::ifstream file { path };
    if (!file.is_open()) exit(1);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    mlang::script::EnvStack env {};

    env.declare_function("register_observer", &observerRegisterFunc);
    env.declare_function("wait_for_notification", &notificationWaitFunc);
    env.declare_function("set_parameter", &setParamFunc);
    env.declare_function("sleep", &sleepFunc);

    mlang::script::Script script { buffer.str() };
    script.execute(env);
}


int main(int argc, char* argv[]) {
    executable_path = std::string{ argv[0] };

    std::thread t1 ( script_1 );
    std::thread t2 ( script_2 );

    t1.join();
    t2.join();
    
    return 0;
}