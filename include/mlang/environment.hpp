#pragma once

#include "mlang/object/object.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/boolean.hpp"
#include "mlang/object/array.hpp"
//#include "mlang/function.hpp"
#include "mlang/exception.hpp"
#include "mlang/ast/function.hpp"

#include <map>
#include <stack>
#include <memory>
#include <string>

namespace mlang {

// forward declare function node
class FunctionDeclNode;

class Environment {
private:
    static inline std::map<std::string, std::shared_ptr<object::ObjectFactory>> m_types { { object::None::type_name, std::make_shared<object::NoneFactory>() },
                                                                                          { object::Number::type_name, std::make_shared<object::NumberFactory>() },
                                                                                          { object::Boolean::type_name, std::make_shared<object::BooleanFactory>() },
                                                                                          //{ object::Array::type_name, std::make_shared<object::ArrayFactory>() },
                                                                                          { object::String::type_name, std::make_shared<object::StringFactory>() }   };
    std::map<std::string, object::Object> m_variables;
    std::map<std::string, const ast::Function*> m_functions;

    Environment* m_parent { nullptr };
public:
    Environment () = default;
    Environment (Environment* parent) : m_parent(parent) {}
    ~Environment () = default;

    void reset () {
        //m_types.clear();
        m_variables.clear();
        m_functions.clear();
        m_parent = nullptr;
    }

    static bool has_type (const std::string& type_name) {
        if (m_types.count(type_name) != 0) { return true; }
        else { return false; }
    }

    static void define_type (const std::string& type_name, std::shared_ptr<object::ObjectFactory> factory) {
        if (has_type(type_name)) { throw RuntimeError{"type " + type_name + " already exists"}; }
        m_types[type_name] = factory;
    }

    static const object::ObjectFactory& get_factory (const std::string& type) {
        if (m_types.count(type) == 0) { throw RuntimeError{"type " + type + " is unknown"}; }
        return *(m_types[type]);
    }

    bool has_variable (const std::string& variable_name) const {
        if (m_variables.count(variable_name) != 0) { return true; }
        else if (m_parent != nullptr) { return m_parent->has_variable(variable_name); }
        else { return false; }
    }

    void declare_variable (const std::string& variable_name, const std::string& type) {
        if (m_types.count(type) == 0) {
            throw RuntimeError{"type " + type + " is unknown"};
        }
        if (has_variable(variable_name)) {
            throw RuntimeError{"variable " + variable_name + " already exists"};
        }
        m_variables[variable_name] = object::Object{m_types[type]->create()};
    }

    object::Object& get_variable (const std::string& variable_name) {
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

    /*
    void set_variable (const std::string& variable_name, Object value) {
        if (m_variables.count(variable_name) != 0) {
            std::string type_name = value->get_typename();
            if (!has_type(type_name)) {
                throw RuntimeError{"type " + type_name + " is unknown"};
            }
            m_variables[variable_name] = m_types[type_name]->create();
            m_variables[variable_name]->assign(value);
        }
        else if (m_parent != nullptr) {
            m_parent->set_variable(variable_name, value);
        }
        else {
            throw RuntimeError{"variable " + variable_name + " does not exists"};
        }
    }
    */

    bool has_function (const std::string& function_name) const {
        if (m_functions.count(function_name) != 0) { return true; }
        else if (m_parent != nullptr) { return m_parent->has_function(function_name); }
        else { return false; }
    }

    void declare_function (const std::string& function_name, const ast::Function* function) {
        if (has_function(function_name)) {
            throw RuntimeError{"function " + function_name + " already exists"};
        }
        m_functions[function_name] = function;
    }

    const ast::Function* get_function (const std::string& function_name) {
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

    void declare_variable (const std::string& variable_name, const std::string& type) {
        m_env_stack.top()->declare_variable(variable_name, type);
    }

    object::Object& get_variable (const std::string& variable_name) {
        return m_env_stack.top()->get_variable(variable_name);
    }

    /*
    void set_variable (const std::string& variable_name, std::shared_ptr<Object> value) {
        return m_env_stack.top()->set_variable(variable_name, value);
    }
    */

    bool has_function (const std::string& function_name) const {
        return m_env_stack.top()->has_function(function_name);
    }

    void declare_function (const std::string& function_name, const ast::Function* function) {
        m_env_stack.top()->declare_function(function_name, function);
    }

    const ast::Function* get_function (const std::string& function_name) {
        return m_env_stack.top()->get_function(function_name);
    }
};

} /* namespace mlang */