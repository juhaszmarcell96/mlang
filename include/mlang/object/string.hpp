#pragma once

#include <string>
#include <algorithm>

#include "mlang/object/object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {

class String : public Object {
private:
    std::string m_value;
public:
    String () : Object(false) {}
    String (bool lvalue) : Object(lvalue) {}
    String (const std::string& value) : Object(false), m_value(value) {}
    String (const std::string& value, bool lvalue) : Object(lvalue), m_value(value) {}
    ~String () = default;
    
    const static inline std::string type_name { "String" };

    const std::string& get () const { return m_value; }

    /* construct */
    void construct (const std::vector<std::shared_ptr<Object>>& params) override {
        if (params.size() == 0) { m_value = ""; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        m_value = str_ptr->get();
    }
    /* assign */
    void assign (const std::vector<std::shared_ptr<Object>>& params) override {
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        m_value = str_ptr->get();
    }
    void assign (const std::shared_ptr<Object> param) override {
        const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
        m_value = str_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = "";
    }

    std::shared_ptr<Object> binary_add (const std::vector<std::shared_ptr<Object>>& params) {
        assert_params(params, 1, type_name, operators::binary_add);
        assert_parameter(params[0], type_name, operators::binary_add);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        return std::make_shared<String>(m_value + str_ptr->get(), false);
    }

    void operator_add_equal (const Object* param) override {
        assert_parameter(param, type_name, "+=");
        const String* str_ptr = assert_cast<const String*>(param, type_name);
        m_value += str_ptr->get();
    }

    std::shared_ptr<Object> comparison_equal (const std::vector<std::shared_ptr<Object>>& params) {
        assert_params(params, 1, type_name, operators::binary_equality);
        assert_parameter(params[0], type_name, operators::binary_equality);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        return std::make_shared<Boolean>(m_value == str_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_not_equal (const std::vector<std::shared_ptr<Object>>& params) {
        assert_params(params, 1, type_name, operators::binary_inequality);
        assert_parameter(params[0], type_name, operators::binary_inequality);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        return std::make_shared<Boolean>(m_value != str_ptr->get(), false);
    }

    std::shared_ptr<Object> reverse () {
        std::string reversed = m_value;
        std::reverse(reversed.begin(), reversed.end());
        return std::make_shared<String>(reversed, false);
    }


    void call (const std::string& func, const std::vector<std::shared_ptr<Object>>& params, std::shared_ptr<Object>& ret_val) override {
        if (func.compare(operators::construct) == 0) {
            construct(params);
        }
        else if (func.compare(operators::assign) == 0) {
            assign(params);
        }
        else if (func.compare(operators::destruct) == 0) {
            destruct();
        }
        else if (func.compare("reverse") == 0) {
            ret_val = reverse();
        }
        else if (func.compare(operators::binary_add) == 0) {
            ret_val = binary_add(params);
        }
        else if (func.compare(operators::binary_equality) == 0) {
            ret_val = comparison_equal(params);
        }
        else if (func.compare(operators::binary_inequality) == 0) {
            ret_val = comparison_not_equal(params);
        }
        else {
            throw RuntimeError { "string object has no " + func + " member function" };
        }
    }

    std::string get_string () const override { return m_value; }
    std::string get_typename () const override { return type_name; }
};

class StringFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<String>();
    }
};

} /* namespace mlang */