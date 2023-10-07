#pragma once

#include <string>

#include "mlang/object/object.hpp"

namespace mlang {

class Number : public Object {
private:
    double m_value;
public:
    Number () : Object(false) {}
    Number (bool lvalue) : Object(lvalue) {}
    Number (const double value) : Object(false), m_value(value) {}
    Number (const double value, bool lvalue) : Object(lvalue), m_value(value) {}
    ~Number () = default;

    const double get () const { return m_value; }

    /* construct */
    void construct (const std::vector<Object*>& params) override {
        if (params.size() == 0) {
            /* default constructor */
            m_value = 0.0;
        }
        else if (params.size() == 1) {
            /* copy constructor */
            if (params[0] == nullptr) {
                throw RuntimeError { "number copy constructor parameter is null" };
            }
            Number* num_ptr = dynamic_cast<Number*>(params[0]);
            if (num_ptr == nullptr) {
                throw RuntimeError { "number copy constructor parameter is not a number object" };
            }
            m_value = num_ptr->get();
        }
        else {
            throw RuntimeError { "Too many arguments in number constructor" };
        }
    }
    /* assign */
    Object* assign (const std::vector<Object*>& params) override {
        if (params.size() == 1) {
            /* copy assignment */
            if (params[0] == nullptr) {
                throw RuntimeError { "number assignment parameter is null" };
            }
            Number* num_ptr = dynamic_cast<Number*>(params[0]);
            if (num_ptr == nullptr) {
                throw RuntimeError { "number assignment parameter is not a number object" };
            }
            m_value = num_ptr->get();
        }
        else {
            throw RuntimeError { "invalid number of arguments in number assignment" };
        }
        return this;
    }
    void assign (const Object* param) override {
        if (param == nullptr) {
            throw RuntimeError { "assignment argument is null" };
        }
        const Number* num_ptr = dynamic_cast<const Number*>(param);
        m_value = num_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = 0.0;
    }

    void binary_add (const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) {
        if (params.size() != 1) { throw RuntimeError { "no operand found for binary addition" }; }
        Number* num_ptr = dynamic_cast<Number*>(params[0]);
        if (num_ptr == nullptr) { throw RuntimeError { "invalid type for addition" }; }
        ret_val = std::make_shared<Number>(m_value + num_ptr->get(), false);
    }

    void binary_sub (const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) {
        if (params.size() != 1) { throw RuntimeError { "no operand found for binary subtraction" }; }
        Number* num_ptr = dynamic_cast<Number*>(params[0]);
        if (num_ptr == nullptr) { throw RuntimeError { "invalid type for subtraction" }; }
        ret_val = std::make_shared<Number>(m_value - num_ptr->get(), false);
    }

    void binary_mul (const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) {
        if (params.size() != 1) { throw RuntimeError { "no operand found for binary addition" }; }
        Number* num_ptr = dynamic_cast<Number*>(params[0]);
        if (num_ptr == nullptr) { throw RuntimeError { "invalid type for multiplication" }; }
        ret_val = std::make_shared<Number>(m_value * num_ptr->get(), false);
    }

    void binary_div (const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) {
        if (params.size() != 1) { throw RuntimeError { "no operand found for binary addition" }; }
        Number* num_ptr = dynamic_cast<Number*>(params[0]);
        if (num_ptr == nullptr) { throw RuntimeError { "invalid type for division" }; }
        ret_val = std::make_shared<Number>(m_value / num_ptr->get(), false);
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
        else if (func.compare("binary_add") == 0) {
            this->binary_add(params, ret_val);
        }
        else if (func.compare("binary_sub") == 0) {
            this->binary_sub(params, ret_val);
        }
        else if (func.compare("binary_mul") == 0) {
            this->binary_mul(params, ret_val);
        }
        else if (func.compare("binary_div") == 0) {
            this->binary_div(params, ret_val);
        }
        else {
            throw RuntimeError { "object of type 'number' has no '" + func + "' member function" };
        }
    }

    std::string get_string () const override { return std::to_string(m_value); }
    std::string get_typename () const override { return "number"; }
    double get_number () const override { return m_value; }
};

class NumberFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<Number>();
    }
};

} /* namespace mlang */