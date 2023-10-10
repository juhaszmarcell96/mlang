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
    Object () { m_object = std::make_shared<None>(); };
    Object (bool lvalue) : m_lvalue(lvalue) { m_object = std::make_shared<None>(); };
    Object (std::shared_ptr<InternalObject> obj) : m_object(obj) { /* TODO : lvalue? */}
    Object (const ObjectFactory& factory) { m_object = factory.create(); }
    ~Object () = default;

    Object call (const std::string& func, const std::vector<Object>& params) {
        
    }

    void construct (const std::vector<Object>& params) {
        /* TODO */
    }
    void assign (const Object& param, const ObjectFactory& factory) {
        m_object = factory.create();
        //m_object->assign(param.m_object);
    }
    void destruct () {} /* reallocate to None */
    std::string get_typename () const { return m_object->get_typename(); }

    bool is_lvalue () const { return m_lvalue; }
    void set_lvalue (bool lvalue) { m_lvalue = lvalue; }

    bool is_true () const { return m_object->is_true(); }
    double get_number () const { return m_object->get_number(); }
    std::string get_string () const { return m_object->get_string(); }

    /* += */
    Object& operator_add_equal (const Object& rhs, const ObjectFactory& factory) {
        m_object = factory.create();
        m_object->operator_add_equal(rhs.m_object.get());
        return *this;
    }

    /* -= */
    Object& operator_sub_equal (const Object& rhs, const ObjectFactory& factory) {
        m_object = factory.create();
        m_object->operator_sub_equal(rhs.m_object.get());
        return *this;
    }

    /* *= */
    Object& operator_mul_equal (const Object& rhs, const ObjectFactory& factory) {
        m_object = factory.create();
        m_object->operator_mul_equal(rhs.m_object.get());
        return *this;
    }

    /* /= */
    Object& operator_div_equal (const Object& rhs, const ObjectFactory& factory) {
        m_object = factory.create();
        m_object->operator_div_equal(rhs.m_object.get());
        return *this;
    }
    
    /* + */
    Object operator_binary_add (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_binary_add(rhs.m_object.get());
        return ret;
    }
    Object operator+(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_add(rhs.m_object.get());
        return ret;
    }
    
    /* - */
    Object operator_binary_sub (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_binary_sub(rhs.m_object.get());
        return ret;
    }
    Object operator-(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_sub(rhs.m_object.get());
        return ret;
    }
    
    /* * */
    Object operator_binary_mul (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_binary_mul(rhs.m_object.get());
        return ret;
    }
    Object operator*(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_mul(rhs.m_object.get());
        return ret;
    }
    
    /* / */
    Object operator_binary_div (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_binary_div(rhs.m_object.get());
        return ret;
    }
    Object operator/(const Object& rhs) const {
        Object ret { false };
        ret.m_object = m_object->operator_binary_div(rhs.m_object.get());
        return ret;
    }

    /* unary - */
    Object unary_minus () {
        Object ret { false };
        ret.m_object = m_object->unary_minus();
        return ret;
    }

    /* unary ! */
    Object unary_not () {
        Object ret { false };
        ret.m_object = m_object->unary_not();
        return ret;
    }

    /* == */
    friend bool operator==(const Object& lhs, const Object& rhs) {
        return lhs.m_object->operator_comparison_equal(rhs.m_object.get())->is_true();
    }
    Object operator_comparison_equal (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_comparison_equal(rhs.m_object.get());
        return ret;
    }

    /* != */
    Object operator_comparison_not_equal (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_comparison_not_equal(rhs.m_object.get());
        return ret;
    }

    /* > */
    Object operator_greater (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_greater(rhs.m_object.get());
        return ret;
    }

    /* < */
    Object operator_less (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_less(rhs.m_object.get());
        return ret;
    }

    /* >= */
    Object operator_greater_equal (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_greater_equal(rhs.m_object.get());
        return ret;
    }

    /* <= */
    Object operator_less_equal (const Object& rhs) {
        Object ret { false };
        ret.m_object = m_object->operator_less_equal(rhs.m_object.get());
        return ret;
    }

    /* [] */
    Object operator_subscript (const Object& param) {
        /* TODO */
        return Object {};
        //return m_object->operator_subscript(param);
    }
};

} /* namespace mlang */