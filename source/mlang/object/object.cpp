#include "mlang/object/object.hpp"
#include "mlang/object/assert.hpp"
#include "mlang/object/boolean.hpp"

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
    std::vector<std::shared_ptr<InternalObject>> internal_params;
    for (const Object& o : params) {
        internal_params.push_back(o.m_object);
    }
    return Object { m_object->call(func, internal_params) };
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
Object& Object::operator_add_equal (const Object& rhs) {
    m_object->operator_add_equal(rhs.m_object);
    return *this;
}

/* -= */
Object& Object::operator_sub_equal (const Object& rhs) {
    m_object->operator_sub_equal(rhs.m_object);
    return *this;
}

/* *= */
Object& Object::operator_mul_equal (const Object& rhs) {
    m_object->operator_mul_equal(rhs.m_object);
    return *this;
}

/* /= */
Object& Object::operator_div_equal (const Object& rhs) {
    m_object->operator_div_equal(rhs.m_object);
    return *this;
}

/* + */
Object Object::operator_binary_add (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_add(rhs.m_object);
    return ret;
}
Object Object::operator+(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_add(rhs.m_object);
    return ret;
}

/* - */
Object Object::operator_binary_sub (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_sub(rhs.m_object);
    return ret;
}
Object Object::operator-(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_sub(rhs.m_object);
    return ret;
}

/* * */
Object Object::operator_binary_mul (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_mul(rhs.m_object);
    return ret;
}
Object Object::operator*(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_mul(rhs.m_object);
    return ret;
}

/* / */
Object Object::operator_binary_div (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_binary_div(rhs.m_object);
    return ret;
}
Object Object::operator/(const Object& rhs) const {
    Object ret { false };
    ret.m_object = m_object->operator_binary_div(rhs.m_object);
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

/* postfix ++ */
Object Object::postfix_increment () {
    Object ret { false };
    ret.assign(*this);
    m_object->increment();
    return ret;
}

/* postfix -- */
Object Object::postfix_decrement () {
    Object ret { false };
    ret.assign(*this);
    m_object->decrement();
    return ret;
}

/* prefix ++ */
Object& Object::prefix_increment () {
    m_object->increment();
    return *this;
}

/* prefix -- */
Object& Object::prefix_decrement () {
    m_object->decrement();
    return *this;
}

/* == */
bool operator==(const Object& lhs, const Object& rhs) {
    return lhs.m_object->operator_comparison_equal(rhs.m_object)->is_true();
}
Object Object::operator_comparison_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_comparison_equal(rhs.m_object);
    return ret;
}

/* != */
Object Object::operator_comparison_not_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_comparison_not_equal(rhs.m_object);
    return ret;
}

/* > */
Object Object::operator_greater (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_greater(rhs.m_object);
    return ret;
}

/* < */
Object Object::operator_less (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_less(rhs.m_object);
    return ret;
}

/* >= */
Object Object::operator_greater_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_greater_equal(rhs.m_object);
    return ret;
}

/* <= */
Object Object::operator_less_equal (const Object& rhs) {
    Object ret { false };
    ret.m_object = m_object->operator_less_equal(rhs.m_object);
    return ret;
}

/* [] */
Object& Object::operator_subscript (const Object& param) {
    return m_object->operator_subscript(param.m_object);
}

/* && */
Object Object::operator_binary_and (const Object& rhs) {
    bool value = is_true() && rhs.is_true();
    return Object { std::make_shared<Boolean>(value) };
}

/* || */
Object Object::operator_binary_or (const Object& rhs) {
    bool value = is_true() || rhs.is_true();
    return Object { std::make_shared<Boolean>(value) };
}

} /* namespace object */
} /* namespace mlang */