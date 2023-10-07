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
    void construct (const std::vector<Object*>& params) override {
        if (params.size() == 0) { m_value = 0.0; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        m_value = num_ptr->get();
    }
    /* assign */
    void assign (const std::vector<Object*>& params) override {
        assert_params(params, 1, type_name, operators::assign);
        assert_parameter(params[0], type_name, operators::assign);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        m_value = num_ptr->get();
    }
    void assign (const Object* param) override {
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value = num_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = 0.0;
    }

    bool is_true () const override {
        return m_value != 0;
    }

    std::shared_ptr<Object> binary_add (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_add);
        assert_parameter(params[0], type_name, operators::binary_add);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Number>(m_value + num_ptr->get(), false);
    }

    std::shared_ptr<Object> binary_sub (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_sub);
        assert_parameter(params[0], type_name, operators::binary_sub);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Number>(m_value - num_ptr->get(), false);
    }

    std::shared_ptr<Object> binary_mul (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_mul);
        assert_parameter(params[0], type_name, operators::binary_mul);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Number>(m_value * num_ptr->get(), false);
    }

    std::shared_ptr<Object> binary_div (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_div);
        assert_parameter(params[0], type_name, operators::binary_div);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Number>(m_value / num_ptr->get(), false);
    }

    void add_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::add_equal);
        assert_parameter(params[0], type_name, operators::add_equal);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        m_value += num_ptr->get();
    }

    void sub_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::sub_equal);
        assert_parameter(params[0], type_name, operators::sub_equal);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        m_value -= num_ptr->get();
    }

    void mul_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::mul_equal);
        assert_parameter(params[0], type_name, operators::mul_equal);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        m_value *= num_ptr->get();
    }

    void div_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::div_equal);
        assert_parameter(params[0], type_name, operators::div_equal);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        m_value /= num_ptr->get();
    }

    std::shared_ptr<Object> comparison_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_equality);
        assert_parameter(params[0], type_name, operators::binary_equality);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value == num_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_not_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_inequality);
        assert_parameter(params[0], type_name, operators::binary_inequality);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value != num_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_greater (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::comparison_greater);
        assert_parameter(params[0], type_name, operators::comparison_greater);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value > num_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_less (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::comparison_less);
        assert_parameter(params[0], type_name, operators::comparison_less);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value < num_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_greater_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::comparison_greater_equal);
        assert_parameter(params[0], type_name, operators::comparison_greater_equal);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value >= num_ptr->get(), false);
    }

    std::shared_ptr<Object> comparison_less_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::comparison_less_equal);
        assert_parameter(params[0], type_name, operators::comparison_less_equal);
        const Number* num_ptr = assert_cast<const Number*>(params[0], type_name);
        return std::make_shared<Boolean>(m_value <= num_ptr->get(), false);
    }

    std::shared_ptr<Object> unary_minus () {
        return std::make_shared<Number>(-m_value, false);
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
        else if (func.compare(operators::binary_sub) == 0) {
            ret_val = binary_sub(params);
        }
        else if (func.compare(operators::binary_mul) == 0) {
            ret_val = binary_mul(params);
        }
        else if (func.compare(operators::binary_div) == 0) {
            ret_val = binary_div(params);
        }
        else if (func.compare(operators::add_equal) == 0) {
            add_equal(params);
        }
        else if (func.compare(operators::sub_equal) == 0) {
            sub_equal(params);
        }
        else if (func.compare(operators::mul_equal) == 0) {
            mul_equal(params);
        }
        else if (func.compare(operators::div_equal) == 0) {
            div_equal(params);
        }
        else if (func.compare(operators::binary_equality) == 0) {
            ret_val = comparison_equal(params);
        }
        else if (func.compare(operators::binary_inequality) == 0) {
            ret_val = comparison_not_equal(params);
        }
        else if (func.compare(operators::comparison_greater) == 0) {
            ret_val = comparison_greater(params);
        }
        else if (func.compare(operators::comparison_less) == 0) {
            ret_val = comparison_less(params);
        }
        else if (func.compare(operators::comparison_greater_equal) == 0) {
            ret_val = comparison_greater_equal(params);
        }
        else if (func.compare(operators::comparison_less_equal) == 0) {
            ret_val = comparison_less_equal(params);
        }
        else if (func.compare(operators::unary_minus) == 0) {
            ret_val = unary_minus();
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