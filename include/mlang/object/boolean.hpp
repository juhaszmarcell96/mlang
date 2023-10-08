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
    void construct (const std::vector<std::shared_ptr<Object>>& params) override {
        if (params.size() == 0) { m_value = false; }
        assert_params(params, 1, type_name, operators::construct);
        assert_parameter(params[0], type_name, operators::construct);
        const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(params[0], type_name);
        m_value = bool_ptr->get();
    }
    /* assign */
    void assign (const std::vector<std::shared_ptr<Object>>& params) override {
        assert_params(params, 1, type_name, operators::assign);
        assert_parameter(params[0], type_name, operators::assign);
        const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(params[0], type_name);
        m_value = bool_ptr->get();
    }
    void assign (const std::shared_ptr<Object> param) override {
        const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(param, type_name);
        m_value = bool_ptr->get();
    }
    /* destruct */
    void destruct () override {
        m_value = false;
    }

    bool is_true () const override {
        return m_value;
    }

    std::shared_ptr<Object> operator_comparison_equal (const Object* param) override {
        assert_parameter(param, type_name, "==");
        return std::make_shared<Boolean>(m_value == param->is_true(), false);
    }

    std::shared_ptr<Object> operator_comparison_not_equal (const Object* param) {
        assert_parameter(param, type_name, "!=");
        return std::make_shared<Boolean>(m_value != param->is_true(), false);
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