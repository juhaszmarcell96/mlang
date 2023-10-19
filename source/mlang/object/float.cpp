#include <string>

#include "mlang/object/float.hpp"
#include "mlang/object/int.hpp"
#include "mlang/object/string.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

Float::Float (const double value) : m_value(value) {}

const ObjectFactory& Float::get_factory () const {
    static FloatFactory factory{};
    return factory;
}

double Float::get () const {
    return m_value;
}

/* construct */
void Float::construct (const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (params.size() == 0) { m_value = 0.0; }
    assert_params(params, 1, type_name, "constructor");
    assert_parameter(params[0], type_name, "constructor");
    m_value = params[0]->get_float();
}

/* assign */
void Float::assign (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "assign");
    m_value = param->get_float();
}

bool Float::is_true () const {
    return m_value != 0.0;
}

std::shared_ptr<InternalObject> Float::operator_binary_add (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+");
    return std::make_shared<Float>(m_value + param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_binary_sub (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "-");
    return std::make_shared<Float>(m_value - param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_binary_mul (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "*");
    return std::make_shared<Float>(m_value * param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_binary_div (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "/");
    return std::make_shared<Float>(m_value / param->get_float());
}

void Float::operator_add_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+=");
    m_value += param->get_float();
}

void Float::operator_sub_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "-=");
    m_value -= param->get_float();
}

void Float::operator_mul_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "*=");
    m_value *= param->get_float();
}

void Float::operator_div_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "/=");
    m_value /= param->get_float();
}

std::shared_ptr<InternalObject> Float::operator_comparison_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "==");
    return std::make_shared<Boolean>(m_value == param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "!=");
    return std::make_shared<Boolean>(m_value != param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_greater (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, ">");
    return std::make_shared<Boolean>(m_value > param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_less (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "<");
    return std::make_shared<Boolean>(m_value < param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_greater_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, ">=");
    return std::make_shared<Boolean>(m_value >= param->get_float());
}

std::shared_ptr<InternalObject> Float::operator_less_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "<=");
    return std::make_shared<Boolean>(m_value <= param->get_float());
}

std::shared_ptr<InternalObject> Float::unary_minus () {
    return std::make_shared<Float>(-m_value);
}

std::shared_ptr<InternalObject> Float::unary_not () {
    return std::make_shared<Boolean>(!m_value);
}

void Float::increment () { ++m_value; }

void Float::decrement () { --m_value; }

std::shared_ptr<InternalObject> Float::to_string () {
    return std::make_shared<String>(std::to_string(m_value));
}

std::shared_ptr<InternalObject> Float::to_int () {
    return std::make_shared<Int>(static_cast<int>(m_value));
}

std::shared_ptr<InternalObject> Float::to_float () {
    return std::make_shared<Float>(m_value);
}

std::shared_ptr<InternalObject> Float::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
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

std::shared_ptr<InternalObject> Float::access (const std::string& member) {
    throw RuntimeError { "object of type '" + type_name + "' has no '" + member + "' member" };
}

std::string Float::get_string () const { return std::to_string(m_value); }
std::string Float::get_typename () const { return type_name; }
int Float::get_int () const { return static_cast<int>(m_value); }
double Float::get_float () const { return m_value; }


std::shared_ptr<InternalObject> FloatFactory::create () const {
    return std::make_shared<Float>();
}

} /* namespace object */
} /* namespace mlang */