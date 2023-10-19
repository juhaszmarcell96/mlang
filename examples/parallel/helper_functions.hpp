#pragma once

#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <map>
#include <mutex>

#include "mlang/object/int.hpp"
#include "mlang/script/script.hpp"
#include "mlang/func/function.hpp"
#include "mlang/script/environment.hpp"

/**
 *  parameters:
 *     - seconds (Int)
 **/
class FuncSleep : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 1) { throw mlang::RuntimeError{ "FuncSleep expects 1 parameters"}; }
        if (params[0].get_typename() != mlang::object::Int::type_name) {
            throw mlang::RuntimeError{ "FuncSleep expects the 1st parameter to be of type " + mlang::object::Int::type_name};
        }
        std::this_thread::sleep_for (std::chrono::seconds(params[0].get_int()));
        return mlang::object::Object {};
    }
};

class ObserverStorage {
private:
    struct Observer {
        std::vector<std::string> parameters;
        std::mutex sync;
    };
    static inline std::map<std::string, std::shared_ptr<Observer>> m_observers;
public:
    static void register_observer (const std::string& observer, const std::string& parameter) {
        if (m_observers.count(observer) == 0) {
            m_observers[observer] = std::make_shared<Observer>();
            m_observers[observer]->parameters.push_back(parameter);
            m_observers[observer]->sync.lock();
        }
        else {
            m_observers[observer]->parameters.push_back(parameter);
        }
    }
    static void notify (const std::string& parameter) {
        for (auto& [key, val] : m_observers) {
            for (const std::string& param : val->parameters) {
                if (param.compare(parameter) == 0) {
                    val->sync.unlock();
                    break;
                }
            }
        }
    }
    static void wait_for (const std::string& observer) {
        m_observers[observer]->sync.lock();
    }
};

/**
 *  parameters:
 *     - observer name (String)
 *     - observed parameter (String)
 **/
class FuncObserverRegister : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 2) { throw mlang::RuntimeError{ "FuncObserverRegister expects 3 parameters"}; }
        if (params[0].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncObserverRegister expects the 1st parameter to be of type " + mlang::object::String::type_name};
        }
        if (params[1].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncObserverRegister expects the 2nd parameter to be of type " + mlang::object::String::type_name};
        }

        ObserverStorage::register_observer(params[0].get_string(), params[1].get_string());

        std::cout << "registered parameter observer '" << params[0].get_string() << "' for parameter '" << params[1].get_string() << "'" << std::endl;

        return mlang::object::Object {};
    }
};

/**
 *  parameters:
 *     - observer name (String)
 **/
class FuncNotificationWait : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 1) { throw mlang::RuntimeError{ "FuncNotificationWait expects 3 parameters"}; }
        if (params[0].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncNotificationWait expects the 1st parameter to be of type " + mlang::object::String::type_name};
        }

        std::cout << "waiting for observer '" << params[0].get_string() << "'" << std::endl;
        ObserverStorage::wait_for(params[0].get_string());

        return mlang::object::Object {};
    }
};

/**
 *  parameters:
 *     - id (String)
 **/
class FuncSetParameter : public mlang::func::Function {
public:
    mlang::object::Object call (mlang::script::EnvStack& env, std::vector<mlang::object::Object>& params) const override {
        if (params.size() != 1) { throw mlang::RuntimeError{ "FuncSetParameter expects 3 parameters"}; }
        if (params[0].get_typename() != mlang::object::String::type_name) {
            throw mlang::RuntimeError{ "FuncSetParameter expects the 1st parameter to be of type " + mlang::object::String::type_name};
        }

        std::cout << "setting value of parameter '" << params[0].get_string() << "', notifying observers" << std::endl;
        ObserverStorage::notify(params[0].get_string());

        return mlang::object::Object {};
    }
};