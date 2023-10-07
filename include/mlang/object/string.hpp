#pragma once

#include <string>

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
    void construct (const std::vector<Object*>& params) override {
        if (params.size() == 0) { m_value = ""; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const String* str_ptr = assert_cast<const String*>(params[0], type_name);
        m_value = str_ptr->get();
    }
    /* assign */
    void assign (const std::vector<Object*>& params) override {
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const String* str_ptr = assert_cast<const String*>(params[0], type_name);
        m_value = str_ptr->get();
    }
    void assign (const Object* param) override {
        const String* str_ptr = assert_cast<const String*>(param, type_name);
        m_value = str_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = "";
    }

    std::shared_ptr<Object> binary_add (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_add);
        assert_parameter(params[0], type_name, operators::binary_add);
        const String* str_ptr = assert_cast<const String*>(params[0], type_name);
        return std::make_shared<String>(m_value + str_ptr->get(), false);
    }

    void add_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::add_equal);
        assert_parameter(params[0], type_name, operators::add_equal);
        const String* str_ptr = assert_cast<const String*>(params[0], type_name);
        m_value += str_ptr->get();
    }

    std::shared_ptr<Object> comparison_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_equality);
        assert_parameter(params[0], type_name, operators::binary_equality);
        const String* str_ptr = assert_cast<const String*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value == str_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_not_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_inequality);
        assert_parameter(params[0], type_name, operators::binary_inequality);
        const String* str_ptr = assert_cast<const String*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value != str_ptr->get(), false);
    }


    void call (const std::string& func, const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) override {
        if (func.compare(operators::construct) == 0) {
            construct(params);
        }
        else if (func.compare(operators::assign) == 0) {
            assign(params);
        }
        else if (func.compare(operators::destruct) == 0) {
            destruct();
        }
        else if (func.compare(operators::binary_add) == 0) {
            ret_val = binary_add(params);
        }
        else if (func.compare(operators::add_equal) == 0) {
            add_equal(params);
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