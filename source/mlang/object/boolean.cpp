#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"

namespace mlang {

class Boolean : public InternalObject {
private:
    bool m_value;
public:
    Boolean () = default;
    Boolean (bool value) : m_value(value) {}
    ~Boolean () = default;

    const static inline std::string type_name { "Boolean" };

    const bool get () const { return m_value; }

    /* construct */
    /*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        if (params.size() == 0) { m_value = false; }
        assert_params(params, 1, type_name, "constructor");
        assert_parameter(params[0], type_name, "constructor");
        m_value = params[0]->is_true();
    }*/
    /* assign */
    /*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        assert_params(params, 1, type_name, operators::assign);
        assert_parameter(params[0], type_name, operators::assign);
        const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(params[0], type_name);
        m_value = bool_ptr->get();
    }
    void assign (const std::shared_ptr<InternalObject> param) override {
        const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(param, type_name);
        m_value = bool_ptr->get();
    }*/
    /* destruct */
    /*void destruct () override {
        m_value = false;
    }*/

    bool is_true () const override {
        return m_value;
    }

    std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) override {
        assert_parameter(param, type_name, "==");
        return std::make_shared<Boolean>(m_value == param->is_true());
    }

    std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) {
        assert_parameter(param, type_name, "!=");
        return std::make_shared<Boolean>(m_value != param->is_true());
    }

    std::shared_ptr<InternalObject> unary_not () override {
        return std::make_shared<Boolean>(!m_value);
    }


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override {
        throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
        return nullptr;
    }

    std::string get_string () const override { return (m_value ? "true" : "false"); }
    std::string get_typename () const override { return type_name; }
    double get_number () const override { return (m_value ? 1 : 0); }
};

class BooleanFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override {
        return std::make_shared<Boolean>();
    }
};

} /* namespace mlang */