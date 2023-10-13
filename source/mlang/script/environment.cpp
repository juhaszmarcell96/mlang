#include "mlang/script/environment.hpp"
#include "mlang/exception.hpp"

namespace mlang {
namespace script {

Environment::Environment (Environment* parent) : m_parent(parent) {}

void Environment::reset () {
    m_variables.clear();
    m_functions.clear();
    m_parent = nullptr;
}

bool Environment::has_type (const std::string& type_name) {
    if (m_types.count(type_name) != 0) { return true; }
    else { return false; }
}

void Environment::define_type (const std::string& type_name, std::shared_ptr<object::ObjectFactory> factory) {
    if (has_type(type_name)) { throw RuntimeError{"type " + type_name + " already exists"}; }
    m_types[type_name] = factory;
}

const object::ObjectFactory& Environment::get_factory (const std::string& type) {
    if (m_types.count(type) == 0) { throw RuntimeError{"type " + type + " is unknown"}; }
    return *(m_types[type]);
}

bool Environment::has_variable (const std::string& variable_name) const {
    if (m_variables.count(variable_name) != 0) { return true; }
    else if (m_parent != nullptr) { return m_parent->has_variable(variable_name); }
    else { return false; }
}

void Environment::declare_variable (const std::string& variable_name, const std::string& type) {
    if (m_types.count(type) == 0) {
        throw RuntimeError{"type " + type + " is unknown"};
    }
    if (has_variable(variable_name)) {
        throw RuntimeError{"variable " + variable_name + " already exists"};
    }
    m_variables[variable_name] = object::Object{m_types[type]->create()};
}

object::Object& Environment::get_variable (const std::string& variable_name) {
    if (m_variables.count(variable_name) != 0) {
        return m_variables[variable_name];
    }
    else if (m_parent != nullptr) {
        return m_parent->get_variable(variable_name);
    }
    else {
        throw RuntimeError{"variable " + variable_name + " does not exists"};
    }
}

bool Environment::has_function (const std::string& function_name) const {
    if (m_functions.count(function_name) != 0) { return true; }
    else if (m_parent != nullptr) { return m_parent->has_function(function_name); }
    else { return false; }
}

void Environment::declare_function (const std::string& function_name, const func::Function* function) {
    if (has_function(function_name)) {
        throw RuntimeError{"function " + function_name + " already exists"};
    }
    m_functions[function_name] = function;
}

const func::Function* Environment::get_function (const std::string& function_name) {
    if (m_functions.count(function_name) != 0) {
        return m_functions[function_name];
    }
    else if (m_parent != nullptr) {
        return m_parent->get_function(function_name);
    }
    else {
        throw RuntimeError{"function " + function_name + " does not exists"};
    }
}






EnvStack::EnvStack () {
    /* create the global environment */
    m_env_stack.push(std::make_unique<Environment>());
}

void EnvStack::enter_scope () {
    m_env_stack.push(std::make_unique<Environment>(m_env_stack.top().get()));
}

void EnvStack::exit_scope () {
    m_env_stack.pop();
}

bool EnvStack::has_variable (const std::string& variable_name) const {
    return m_env_stack.top()->has_variable(variable_name);
}

void EnvStack::declare_variable (const std::string& variable_name, const std::string& type) {
    m_env_stack.top()->declare_variable(variable_name, type);
}

object::Object& EnvStack::get_variable (const std::string& variable_name) {
    return m_env_stack.top()->get_variable(variable_name);
}

bool EnvStack::has_function (const std::string& function_name) const {
    return m_env_stack.top()->has_function(function_name);
}

void EnvStack::declare_function (const std::string& function_name, const func::Function* function) {
    m_env_stack.top()->declare_function(function_name, function);
}

const func::Function* EnvStack::get_function (const std::string& function_name) {
    return m_env_stack.top()->get_function(function_name);
}

} /* namespace script */
} /* namespace mlang */