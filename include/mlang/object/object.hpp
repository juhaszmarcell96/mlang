#pragma once

#include "mlang/object/internal_object.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/boolean.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace mlang {

/* wrapper around the internal object */
class Object {
protected:
    bool m_lvalue { false };
    std::shared_ptr<InternalObject> m_object;
public:
    Object () = delete;
    Object (bool lvalue) : m_lvalue(lvalue) {
        m_object = std::make_shared<None>();
    };
    ~Object () = default;

    //Object call (const std::string& func, const std::vector<Object>& params) {
    //    
    //}

    void construct (const std::vector<Object>& params) {
        /* TODO */
    }
    void assign (const Object& param, const ObjectFactory* factory) {
        /* reallocate to whatever comes */
        if (factory == nullptr) { /* TODO : runtime error */ }
        m_object = factory->create();
        m_object->assign(param.m_object);
    }
    void destruct () = 0; /* reallocate to None */
    std::string get_typename () const { return m_object->get_typename(); }

    bool is_lvalue () const { return m_lvalue; }
    void set_lvalue (bool lvalue) { m_lvalue = lvalue; }

    bool is_true () const { return m_object->is_true(); }
    double get_number () const { return m_object->get_number(); }
    std::string get_string () const { return m_object->get_number(); }

    /* += */
    Object& operator+=(const Object& rhs) {
        m_object = m_object->operator_add_equal(rhs.m_object);
        return *this;
    }

    /* -= */
    Object& operator-=(const Object& rhs) {
        m_object = m_object->operator_sub_equal(rhs.m_object);
        return *this;
    }

    /* *= */
    Object& operator*=(const Object& rhs) {
        m_object = m_object->operator_mul_equal(rhs.m_object);
        return *this;
    }

    /* /= */
    Object& operator/=(const Object& rhs) {
        m_object = m_object->operator_div_equal(rhs.m_object);
        return *this;
    }
    
    /* + */
    Object operator+(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_add(rhs.m_object);
        return ret;
    }
    
    /* - */
    Object operator-(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_sub(rhs.m_object);
        return ret;
    }
    
    /* * */
    Object operator*(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_mul(rhs.m_object);
        return ret;
    }
    
    /* / */
    Object operator/(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_div(rhs.m_object);
        return ret;
    }

    /* unary - */
    std::shared_ptr<Object> unary_minus () { return m_object->unary_minus(); }

    /* unary ! */
    std::shared_ptr<Object> unary_not () { return m_object->unary_not(); }

    /* == */
    friend bool operator==(const Object& lhs, const Object& rhs) {
        return lhs.m_object->operator_comparison_equal(rhs->m_object)->is_true();
    }
    Object operator_comparison_equal (const Object* rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_comparison_equal(rhs->m_object);
        return ret;
    }

    /* != */
    std::shared_ptr<Object> operator_comparison_not_equal (const Object* param) { return m_object->operator_comparison_not_equal(param); }

    /* > */
    std::shared_ptr<Object> operator_greater (const Object* param) { return m_object->operator_greater(param); }

    /* < */
    std::shared_ptr<Object> operator_less (const Object* param) { return m_object->operator_less(param); }

    /* >= */
    std::shared_ptr<Object> operator_greater_equal (const Object* param) { return m_object->operator_greater_equal(param); }

    /* <= */
    std::shared_ptr<Object> operator_less_equal (const Object* param) { return m_object->operator_less_equal(param); }

    /* [] */
    std::shared_ptr<Object> operator_array_index (const Object* param) { return m_object->operator_array_index(param); }
};

} /* namespace mlang */