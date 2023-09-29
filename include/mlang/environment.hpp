#pragma once

#include "value.hpp"
#include "exception.hpp"

#include <map>
#include <memory>
#include <string>

namespace mlang {

class Environment {
private:
    std::map<std::string, std::shared_ptr<Value>> m_variables;
    //std::map<std::string, std::shared_ptr<Function>> m_functions;
public:
    Environment () = default;
    ~Environment () = default;

    void reset () {
        m_variables.clear();
    }

    bool has_variable (const std::string& variable_name) const {
        return m_variables.count(variable_name) > 0;
    }

    void declare_variable (const std::string& variable_name, value_types type) {
        if (m_variables.count(variable_name) != 0) {
            throw semantics_error{"variable " + variable_name + " already exists"};
        }
        m_variables[variable_name] = std::make_shared<Value>(type);
    }

    Value* get_variable (const std::string& variable_name) {
        if (m_variables.count(variable_name) == 0) {
            throw semantics_error{"variable " + variable_name + " does not exists"};
        }
        return m_variables[variable_name].get();
    }
};

} /* namespace mlang */