#pragma once

#include <string>

#include "mlang/object/object.hpp"

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

    const std::string& get () const { return m_value; }

    /* construct */
    void construct (const std::vector<Object*>& params) override {
        if (params.size() == 0) {
            /* default constructor */
            m_value = "";
        }
        else if (params.size() == 1) {
            /* copy constructor */
            if (params[0] == nullptr) {
                throw RuntimeError { "string copy constructor parameter is null" };
            }
            String* str_ptr = dynamic_cast<String*>(params[0]);
            if (str_ptr == nullptr) {
                throw RuntimeError { "string copy constructor parameter is not a string object" };
            }
            m_value = str_ptr->get();
        }
        else {
            throw RuntimeError { "Too many arguments in string constructor" };
        }
    }
    /* assign */
    Object* assign (const std::vector<Object*>& params) override {
        if (params.size() == 1) {
            /* copy assignment */
            if (params[0] == nullptr) {
                throw RuntimeError { "string assignment parameter is null" };
            }
            String* str_ptr = dynamic_cast<String*>(params[0]);
            if (str_ptr == nullptr) {
                throw RuntimeError { "string assignment parameter is not a string object" };
            }
            m_value = str_ptr->get();
        }
        else {
            throw RuntimeError { "invalid number of arguments in string assignment" };
        }
        return this;
    }
    void assign (const Object* param) override {
        if (param == nullptr) {
            throw RuntimeError { "assignment argument is null" };
        }
        const String* str_ptr = dynamic_cast<const String*>(param);
        m_value = str_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = "";
    }


    void call (const std::string& func, const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) override {
        if (func.compare("construct") == 0) {
            this->construct(params);
        }
        //else if (func.compare("assign") == 0) {
        //    ret_val = *(this->assign(params));
        //}
        else if (func.compare("destruct") == 0) {
            this->destruct();
        }
        else {
            throw RuntimeError { "string object has no " + func + " member function" };
        }
    }

    std::string get_string () const override { return m_value; }
    std::string get_typename () const override { return "string"; }
};

class StringFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<String>();
    }
};

} /* namespace mlang */