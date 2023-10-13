#pragma once

#include "mlang/object/object.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/boolean.hpp"
#include "mlang/object/array.hpp"

#include "mlang/func/function.hpp"

#include <map>
#include <stack>
#include <memory>
#include <string>

namespace mlang {
namespace script {

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
    std::map<std::string, const func::Function*> m_functions;

    Environment* m_parent { nullptr };
public:
    Environment () = default;
    Environment (Environment* parent);
    ~Environment () = default;

    void reset ();

    static bool has_type (const std::string& type_name);
    static void define_type (const std::string& type_name, std::shared_ptr<object::ObjectFactory> factory);
    static const object::ObjectFactory& get_factory (const std::string& type);

    bool has_variable (const std::string& variable_name) const;
    void declare_variable (const std::string& variable_name, const std::string& type);
    object::Object& get_variable (const std::string& variable_name);

    bool has_function (const std::string& function_name) const;
    void declare_function (const std::string& function_name, const func::Function* function);
    const func::Function* get_function (const std::string& function_name);
};

class EnvStack {
private:
    std::stack<std::unique_ptr<Environment>> m_env_stack;
public:
    EnvStack ();

    void enter_scope ();
    void exit_scope ();

    bool has_variable (const std::string& variable_name) const;
    void declare_variable (const std::string& variable_name, const std::string& type);
    object::Object& get_variable (const std::string& variable_name);

    bool has_function (const std::string& function_name) const;
    void declare_function (const std::string& function_name, const func::Function* function);
    const func::Function* get_function (const std::string& function_name);
};

} /* namespace script */
} /* namespace mlang */