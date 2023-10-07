#pragma once

#include <string>

#include "mlang/object/object.hpp"

namespace mlang {

class Boolean : public Object {
private:
    bool m_value;
public:
    Boolean () : Object(false) {}
    Boolean (bool value) : Object(false), m_value(value) {}
    Boolean (bool value, bool lvalue) : Object(lvalue), m_value(value) {}
    ~Boolean () = default;

    const static inline std::string type_name { "Boolean" };

    const bool get () const { return m_value; }

    /* construct */
    void construct (const std::vector<Object*>& params) override {
        if (params.size() == 0) { m_value = false; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const Boolean* bool_ptr = assert_cast<const Boolean*>(params[0], type_name);
        m_value = bool_ptr->get();
    }
    /* assign */
    void assign (const std::vector<Object*>& params) override {
        assert_params(params, 1, type_name, operators::assign);
        assert_parameter(params[0], type_name, operators::assign);
        const Boolean* bool_ptr = assert_cast<const Boolean*>(params[0], type_name);
        m_value = bool_ptr->get();
    }
    void assign (const Object* param) override {
        const Boolean* bool_ptr = assert_cast<const Boolean*>(param, type_name);
        m_value = bool_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = false;
    }

    bool is_true () const override {
        return m_value;
    }

    std::shared_ptr<Object> comparison_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_equality);
        assert_parameter(params[0], type_name, operators::binary_equality);
        return std::make_shared<Boolean>(m_value == params[0]->is_true(), false);
    }

    std::shared_ptr<Object> comparison_not_equal (const std::vector<Object*>& params) {
        assert_params(params, 1, type_name, operators::binary_inequality);
        assert_parameter(params[0], type_name, operators::binary_inequality);
        return std::make_shared<Boolean>(m_value != params[0]->is_true(), false);
    }

    std::shared_ptr<Object> unary_not () {
        return std::make_shared<Boolean>(!m_value, false);
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
        else if (func.compare(operators::binary_equality) == 0) {
            ret_val = comparison_equal(params);
        }
        else if (func.compare(operators::binary_inequality) == 0) {
            ret_val = comparison_not_equal(params);
        }
        else if (func.compare(operators::unary_not) == 0) {
            ret_val = unary_not();
        }
        else {
            throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
        }
    }

    std::string get_string () const override { return (m_value ? "true" : "false"); }
    std::string get_typename () const override { return type_name; }
    double get_number () const override { return (m_value ? 1 : 0); }
};

class BooleanFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<Boolean>();
    }
};

} /* namespace mlang */