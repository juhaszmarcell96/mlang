#pragma once

#include <string>
#include <algorithm>

#include "mlang/object/internal_object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {

class String : public InternalObject {
private:
    std::string m_value;
public:
    String () = default;
    String (const std::string& value) : m_value(value) {}
    ~String () = default;
    
    const static inline std::string type_name { "String" };

    const std::string& get () const { return m_value; }

    /* construct */
    /*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        if (params.size() == 0) { m_value = ""; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        m_value = str_ptr->get();
    }*/
    /* assign */
    /*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
        m_value = str_ptr->get();
    }*/
    /*void assign (const std::shared_ptr<InternalObject> param) override {
        const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
        m_value = str_ptr->get();
    }*/
    /* destruct */
    /*void destruct () override {
        m_value = "";
    }*/

    std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param) override {
        assert_parameter(param, type_name, "+");
        const String* str_ptr = assert_cast<const String*>(param, type_name);
        return std::make_shared<String>(m_value + str_ptr->get());
    }

    void operator_add_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "+=");
        const String* str_ptr = assert_cast<const String*>(param, type_name);
        m_value += str_ptr->get();
    }

    std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "==");
        const String* str_ptr = assert_cast<const String*>(param, type_name);
        return std::make_shared<Boolean>(m_value == str_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "!=");
        const String* str_ptr = assert_cast<const String*>(param, type_name);
        return std::make_shared<Boolean>(m_value != str_ptr->get());
    }

    std::shared_ptr<InternalObject> reverse () {
        std::string reversed = m_value;
        std::reverse(reversed.begin(), reversed.end());
        return std::make_shared<String>(reversed);
    }


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override {
        if (func.compare("reverse") == 0) {
            return reverse();
        }
        else {
            throw RuntimeError { "string object has no " + func + " member function" };
        }
        return nullptr;
    }

    std::string get_string () const override { return m_value; }
    std::string get_typename () const override { return type_name; }
};

class StringFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override {
        return std::make_shared<String>();
    }
};

} /* namespace mlang */