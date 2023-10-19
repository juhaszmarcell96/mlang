#include <string>

#include "mlang/object/int.hpp"
#include "mlang/object/float.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

Int::Int (const int value) : m_value(value) {}

const ObjectFactory& Int::get_factory () const {
    static IntFactory factory{};
    return factory;
}

int Int::get () const {
    return m_value;
}

/* construct */
void Int::construct (const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (params.size() == 0) { m_value = 0.0; }
    assert_params(params, 1, type_name, "constructor");
    assert_parameter(params[0], type_name, "constructor");
    m_value = params[0]->get_int();
}

/* assign */
void Int::assign (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "assign");
    m_value = param->get_int();
}

bool Int::is_true () const {
    return m_value != 0;
}

std::shared_ptr<InternalObject> Int::operator_binary_add (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+");
    return std::make_shared<Int>(m_value + param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_binary_sub (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "-");
    return std::make_shared<Int>(m_value - param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_binary_mul (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "*");
    return std::make_shared<Int>(m_value * param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_binary_div (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "/");
    return std::make_shared<Int>(m_value / param->get_int());
}

void Int::operator_add_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+=");
    m_value += param->get_int();
}

void Int::operator_sub_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "-=");
    m_value -= param->get_int();
}

void Int::operator_mul_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "*=");
    m_value *= param->get_int();
}

void Int::operator_div_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "/=");
    m_value /= param->get_int();
}

std::shared_ptr<InternalObject> Int::operator_comparison_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "==");
    return std::make_shared<Boolean>(m_value == param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "!=");
    return std::make_shared<Boolean>(m_value != param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_greater (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, ">");
    return std::make_shared<Boolean>(m_value > param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_less (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "<");
    return std::make_shared<Boolean>(m_value < param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_greater_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, ">=");
    return std::make_shared<Boolean>(m_value >= param->get_int());
}

std::shared_ptr<InternalObject> Int::operator_less_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "<=");
    return std::make_shared<Boolean>(m_value <= param->get_int());
}

std::shared_ptr<InternalObject> Int::unary_minus () {
    return std::make_shared<Int>(-m_value);
}

std::shared_ptr<InternalObject> Int::unary_not () {
    return std::make_shared<Boolean>(!m_value);
}

void Int::increment () { ++m_value; }

void Int::decrement () { --m_value; }

std::shared_ptr<InternalObject> Int::to_string () {
    return std::make_shared<String>(std::to_string(m_value));
}

std::shared_ptr<InternalObject> Int::to_int () {
    return std::make_shared<Int>(m_value);
}

std::shared_ptr<InternalObject> Int::to_float () {
    return std::make_shared<Float>(m_value);
}

std::shared_ptr<InternalObject> Int::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (func.compare("to_string") == 0) {
        return to_string();
    }
    else if (func.compare("to_int") == 0) {
        return to_int();
    }
    else if (func.compare("to_float") == 0) {
        return to_float();
    }
    else {
        throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
    }
    return nullptr;
}

std::shared_ptr<InternalObject> Int::access (const std::string& member) {
    throw RuntimeError { "object of type '" + type_name + "' has no '" + member + "' member" };
}

std::string Int::get_string () const { return std::to_string(m_value); }
std::string Int::get_typename () const { return type_name; }
int Int::get_int () const { return m_value; }
double Int::get_float () const { return static_cast<double>(m_value); }


std::shared_ptr<InternalObject> IntFactory::create () const {
    return std::make_shared<Int>();
}

} /* namespace object */
} /* namespace mlang */