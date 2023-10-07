#pragma once

#include "mlang/value.hpp"
#include "mlang/function.hpp"
#include "mlang/exception.hpp"

#include <map>
#include <stack>
#include <memory>
#include <string>

namespace mlang {

// forward declare function node
class FunctionDeclNode;

class Environment {
private:
    std::map<std::string, std::shared_ptr<Value>> m_variables;
    std::map<std::string, std::shared_ptr<Function>> m_functions;

    Environment* m_parent { nullptr };
public:
    Environment () = default;
    Environment (Environment* parent) : m_parent(parent) {}
    ~Environment () = default;

    void reset () {
        m_variables.clear();
        m_functions.clear();
        m_parent = nullptr;
    }

    bool has_variable (const std::string& variable_name) const {
        if (m_variables.count(variable_name) != 0) { return true; }
        else if (m_parent != nullptr) { return m_parent->has_variable(variable_name); }
        else { return false; }
    }

    void declare_variable (const std::string& variable_name, value_types type) {
        if (has_variable(variable_name)) {
            throw semantics_error{"variable " + variable_name + " already exists"};
        }
        m_variables[variable_name] = std::make_shared<Value>(type);
    }

    Value* get_variable (const std::string& variable_name) {
        if (m_variables.count(variable_name) != 0) {
            return m_variables[variable_name].get();
        }
        else if (m_parent != nullptr) {
            return m_parent->get_variable(variable_name);
        }
        else {
            throw semantics_error{"variable " + variable_name + " does not exists"};
        }
    }

    bool has_function (const std::string& function_name) const {
        if (m_functions.count(function_name) != 0) { return true; }
        else if (m_parent != nullptr) { return m_parent->has_function(function_name); }
        else { return false; }
    }

    void declare_function (const std::string& function_name, FunctionDeclNode* function) {
        if (has_function(function_name)) {
            throw semantics_error{"function " + function_name + " already exists"};
        }
        m_functions[function_name] = std::make_shared<Function>(function);
    }

    Function* get_function (const std::string& function_name) {
        if (m_functions.count(function_name) != 0) {
            return m_functions[function_name].get();
        }
        else if (m_parent != nullptr) {
            return m_parent->get_function(function_name);
        }
        else {
            throw semantics_error{"function " + function_name + " does not exists"};
        }
    }
};

class EnvStack {
private:
    std::stack<std::unique_ptr<Environment>> m_env_stack;
public:
    EnvStack () {
        /* create the global environment */
        m_env_stack.push(std::make_unique<Environment>());
    }

    void enter_scope () {
        m_env_stack.push(std::make_unique<Environment>(m_env_stack.top().get()));
    }

    void exit_scope () {
        m_env_stack.pop();
    }

    bool has_variable (const std::string& variable_name) const {
        return m_env_stack.top()->has_variable(variable_name);
    }

    void declare_variable (const std::string& variable_name, value_types type) {
        m_env_stack.top()->declare_variable(variable_name, type);
    }

    Value* get_variable (const std::string& variable_name) {
        return m_env_stack.top()->get_variable(variable_name);
    }

    bool has_function (const std::string& function_name) const {
        return m_env_stack.top()->has_function(function_name);
    }

    void declare_function (const std::string& function_name, FunctionDeclNode* function) {
        m_env_stack.top()->declare_function(function_name, function);
    }

    Function* get_function (const std::string& function_name) {
        return m_env_stack.top()->get_function(function_name);
    }
};

} /* namespace mlang */