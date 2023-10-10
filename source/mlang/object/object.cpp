#include "mlang/object/object.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

Object::Object () { m_object = std::make_shared<None>(); };
Object::Object (bool lvalue) : m_lvalue(lvalue) { m_object = std::make_shared<None>(); };
Object::Object (std::shared_ptr<InternalObject> obj) : m_object(obj) { /* TODO : lvalue? */}
Object::Object (const ObjectFactory& factory) { m_object = factory.create(); }

const ObjectFactory& Object::get_factory () const {
    return m_object->get_factory();
}

Object Object::call (const std::string& func, const std::vector<Object>& params) {
    
}

void Object::construct (const std::vector<Object>& params) {
    /* TODO */
}
void Object::assign (const Object& param) {
    m_object = param.get_factory().create();
    m_object->assign(param.m_object);
}
void Object::destruct () {} /* reallocate to None */
std::string Object::get_typename () const { return m_object->get_typename(); }

bool Object::is_lvalue () const { return m_lvalue; }
void Object::set_lvalue (bool lvalue) { m_lvalue = lvalue; }

bool Object::is_true () const { return m_object->is_true(); }
double Object::get_number () const { return m_object->get_number(); }
std::string Object::get_string () const { return m_object->get_string(); }

/* += */
Object& Object::operator_add_equal (const Object& rhs, const ObjectFactory& factory) {
    m_object = factory.create();
    m_object->operator_add_equal(rhs.m_object.get());
    return *this;
}

/* -= */
Object& Object::operator_sub_equal (const Object& rhs, const ObjectFactory& factory) {
    m_object = factory.create();
    m_object->operator_sub_equal(rhs.m_object.get());
    return *this;
}

/* *= */
Object& Object::operator_mul_equal (const Object& rhs, const ObjectFactory& factory) {
    m_object = factory.create();
    m_object->operator_mul_equal(rhs.m_object.get());
    return *this;
}

/* /= */
Object& Object::operator_div_equal (const Object& rhs, const ObjectFactory& factory) {
    m_object = factory.create();
    m_object->operator_div_equal(rhs.m_object.get());
    return *this;
}

/* + */
Object Object::operator_binary_add (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_add(rhs.m_object.get());
    return ret;
}
Object Object::operator+(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_add(rhs.m_object.get());
    return ret;
}

/* - */
Object Object::operator_binary_sub (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_sub(rhs.m_object.get());
    return ret;
}
Object Object::operator-(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_sub(rhs.m_object.get());
    return ret;
}

/* * */
Object Object::operator_binary_mul (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_mul(rhs.m_object.get());
    return ret;
}
Object Object::operator*(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_mul(rhs.m_object.get());
    return ret;
}

/* / */
Object Object::operator_binary_div (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_div(rhs.m_object.get());
    return ret;
}
Object Object::operator/(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_div(rhs.m_object.get());
    return ret;
}

/* unary - */
Object Object::unary_minus () {
    Object ret { false };
    ret.m_object = m_object->unary_minus();
    return ret;
}

/* unary ! */
Object Object::unary_not () {
    Object ret { false };
    ret.m_object = m_object->unary_not();
    return ret;
}

/* == */
bool operator==(const Object& lhs, const Object& rhs) {
    return lhs.m_object->operator_comparison_equal(rhs.m_object.get())->is_true();
}
Object Object::operator_comparison_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_comparison_equal(rhs.m_object.get());
    return ret;
}

/* != */
Object Object::operator_comparison_not_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_comparison_not_equal(rhs.m_object.get());
    return ret;
}

/* > */
Object Object::operator_greater (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_greater(rhs.m_object.get());
    return ret;
}

/* < */
Object Object::operator_less (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_less(rhs.m_object.get());
    return ret;
}

/* >= */
Object Object::operator_greater_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_greater_equal(rhs.m_object.get());
    return ret;
}

/* <= */
Object Object::operator_less_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_less_equal(rhs.m_object.get());
    return ret;
}

/* [] */
Object Object::operator_subscript (const Object& param) {
    /* TODO */
    return Object {};
    //return m_object->operator_subscript(param);
}

} /* namespace object */
} /* namespace mlang */