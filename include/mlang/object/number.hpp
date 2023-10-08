#pragma once

#include <string>

#include "mlang/object/object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {

class Number : public Object {
private:
    double m_value;
public:
    Number () : Object(false) {}
    Number (bool lvalue) : Object(lvalue) {}
    Number (const double value) : Object(false), m_value(value) {}
    Number (const double value, bool lvalue) : Object(lvalue), m_value(value) {}
    Number (const int value) : Object(false), m_value(value) {}
    Number (const int value, bool lvalue) : Object(lvalue), m_value(value) {}
    ~Number () = default;
    
    const static inline std::string type_name { "Number" };

    const double get () const { return m_value; }

    /* construct */
    void construct (const std::vector<std::shared_ptr<Object>>& params) override {
        if (params.size() == 0) { m_value = 0.0; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<Number> num_ptr = assert_cast<Number>(params[0], type_name);
        m_value = num_ptr->get();
    }
    /* assign */
    void assign (const std::vector<std::shared_ptr<Object>>& params) override {
        assert_params(params, 1, type_name, operators::assign);
        assert_parameter(params[0], type_name, operators::assign);
        const std::shared_ptr<Number> num_ptr = assert_cast<Number>(params[0], type_name);
        m_value = num_ptr->get();
    }
    void assign (const std::shared_ptr<Object> param) override {
        const std::shared_ptr<Number> num_ptr = assert_cast<Number>(param, type_name);
        m_value = num_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = 0.0;
    }

    bool is_true () const override {
        return m_value != 0;
    }

    std::shared_ptr<Object> operator_binary_add (const Object* param) override {
        assert_parameter(param, type_name, "+");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value + num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_binary_sub (const Object* param) override {
        assert_parameter(param, type_name, "-");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value - num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_binary_mul (const Object* param) override {
        assert_parameter(param, type_name, "*");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value * num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_binary_div (const Object* param) override {
        assert_parameter(param, type_name, "/");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value / num_ptr->get(), false);
    }

    void operator_add_equal (const Object* param) override {
        assert_parameter(param, type_name, "+=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value += num_ptr->get();
    }

    void operator_sub_equal (const Object* param) override {
        assert_parameter(param, type_name, "-=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value -= num_ptr->get();
    }

    void operator_mul_equal (const Object* param) override {
        assert_parameter(param, type_name, "*=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value *= num_ptr->get();
    }

    void operator_div_equal (const Object* param) override {
        assert_parameter(param, type_name, "/=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value /= num_ptr->get();
    }

    std::shared_ptr<Object> operator_comparison_equal (const Object* param) override {
        assert_parameter(param, type_name, "==");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value == num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_comparison_not_equal (const Object* param) override {
        assert_parameter(param, type_name, "!=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value != num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_greater (const Object* param) override {
        assert_parameter(param, type_name, ">");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value > num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_less (const Object* param) override {
        assert_parameter(param, type_name, "<");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value < num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_greater_equal (const Object* param) override {
        assert_parameter(param, type_name, ">=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value >= num_ptr->get(), false);
    }

    std::shared_ptr<Object> operator_less_equal (const Object* param) override {
        assert_parameter(param, type_name, "<=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value <= num_ptr->get(), false);
    }

    std::shared_ptr<Object> unary_minus () override {
        return std::make_shared<Number>(-m_value, false);
    }

    std::shared_ptr<Object> unary_not () override {
        return std::make_shared<Boolean>(!m_value, false);
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
        else {
            throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
        }
    }

    std::string get_string () const override { return std::to_string(m_value); }
    std::string get_typename () const override { return type_name; }
    double get_number () const override { return m_value; }
};

class NumberFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<Number>();
    }
};

} /* namespace mlang */