#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {

class Number : public InternalObject {
private:
    double m_value;
public:
    Number () = default;
    Number (const double value) : m_value(value) {}
    Number (const int value) : m_value(value) {}
    ~Number () = default;
    
    const static inline std::string type_name { "Number" };

    const double get () const { return m_value; }

    /* construct */
    /*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        if (params.size() == 0) { m_value = 0.0; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<Number> num_ptr = assert_cast<Number>(params[0], type_name);
        m_value = num_ptr->get();
    }*/
    /* assign */
    /*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        assert_params(params, 1, type_name, operators::assign);
        assert_parameter(params[0], type_name, operators::assign);
        const std::shared_ptr<Number> num_ptr = assert_cast<Number>(params[0], type_name);
        m_value = num_ptr->get();
    }*/
    /*void assign (const std::shared_ptr<InternalObject> param) override {
        const std::shared_ptr<Number> num_ptr = assert_cast<Number>(param, type_name);
        m_value = num_ptr->get();
    }*/
    /* destruct */
    /*void destruct () override {
        m_value = 0.0;
    }*/

    bool is_true () const override {
        return m_value != 0;
    }

    std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param) override {
        assert_parameter(param, type_name, "+");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value + num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_binary_sub (const InternalObject* param) override {
        assert_parameter(param, type_name, "-");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value - num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_binary_mul (const InternalObject* param) override {
        assert_parameter(param, type_name, "*");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value * num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_binary_div (const InternalObject* param) override {
        assert_parameter(param, type_name, "/");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Number>(m_value / num_ptr->get());
    }

    void operator_add_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "+=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value += num_ptr->get();
    }

    void operator_sub_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "-=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value -= num_ptr->get();
    }

    void operator_mul_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "*=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value *= num_ptr->get();
    }

    void operator_div_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "/=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        m_value /= num_ptr->get();
    }

    std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "==");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value == num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "!=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value != num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_greater (const InternalObject* param) override {
        assert_parameter(param, type_name, ">");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value > num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_less (const InternalObject* param) override {
        assert_parameter(param, type_name, "<");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value < num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_greater_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, ">=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value >= num_ptr->get());
    }

    std::shared_ptr<InternalObject> operator_less_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "<=");
        const Number* num_ptr = assert_cast<const Number*>(param, type_name);
        return std::make_shared<Boolean>(m_value <= num_ptr->get());
    }

    std::shared_ptr<InternalObject> unary_minus () override {
        return std::make_shared<Number>(-m_value);
    }

    std::shared_ptr<InternalObject> unary_not () override {
        return std::make_shared<Boolean>(!m_value);
    }


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override {
        throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
    }

    std::string get_string () const override { return std::to_string(m_value); }
    std::string get_typename () const override { return type_name; }
    double get_number () const override { return m_value; }
};

class NumberFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override {
        return std::make_shared<Number>();
    }
};

} /* namespace mlang */