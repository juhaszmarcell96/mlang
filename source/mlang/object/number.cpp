#include <string>

#include "mlang/object/number.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

Number::Number (const double value) : m_value(value) {}
Number::Number (const int value) : m_value(value) {}

const ObjectFactory& Number::get_factory () const {
    static NumberFactory factory{};
    return factory;
}

/* construct */
/*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (params.size() == 0) { m_value = 0.0; }
    assert_params(params, 1, type_name, operators::construct);
    assert_parameter(params[0], type_name, operators::construct);
    const std::shared_ptr<Number> num_ptr = assert_cast<Number>(params[0], type_name);
    m_value = num_ptr->get();
}*/
/* assign */
/*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 1, type_name, operators::assign);
    assert_parameter(params[0], type_name, operators::assign);
    const std::shared_ptr<Number> num_ptr = assert_cast<Number>(params[0], type_name);
    m_value = num_ptr->get();
}*/
/**/
/* destruct */
/*void destruct () {
    m_value = 0.0;
}*/
void Number::assign (const std::shared_ptr<InternalObject> param) {
    const std::shared_ptr<Number> num_ptr = assert_cast<Number>(param, type_name);
    m_value = num_ptr->m_value;
}

bool Number::is_true () const {
    return m_value != 0;
}

std::shared_ptr<InternalObject> Number::operator_binary_add (const InternalObject* param) {
    assert_parameter(param, type_name, "+");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Number>(m_value + num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_binary_sub (const InternalObject* param) {
    assert_parameter(param, type_name, "-");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Number>(m_value - num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_binary_mul (const InternalObject* param) {
    assert_parameter(param, type_name, "*");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Number>(m_value * num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_binary_div (const InternalObject* param) {
    assert_parameter(param, type_name, "/");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Number>(m_value / num_ptr->m_value);
}

void Number::operator_add_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "+=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    m_value += num_ptr->m_value;
}

void Number::operator_sub_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "-=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    m_value -= num_ptr->m_value;
}

void Number::operator_mul_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "*=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    m_value *= num_ptr->m_value;
}

void Number::operator_div_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "/=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    m_value /= num_ptr->m_value;
}

std::shared_ptr<InternalObject> Number::operator_comparison_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "==");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Boolean>(m_value == num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_comparison_not_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "!=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Boolean>(m_value != num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_greater (const InternalObject* param) {
    assert_parameter(param, type_name, ">");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Boolean>(m_value > num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_less (const InternalObject* param) {
    assert_parameter(param, type_name, "<");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Boolean>(m_value < num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_greater_equal (const InternalObject* param) {
    assert_parameter(param, type_name, ">=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Boolean>(m_value >= num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::operator_less_equal (const InternalObject* param) {
    assert_parameter(param, type_name, "<=");
    const Number* num_ptr = assert_cast<const Number*>(param, type_name);
    return std::make_shared<Boolean>(m_value <= num_ptr->m_value);
}

std::shared_ptr<InternalObject> Number::unary_minus () {
    return std::make_shared<Number>(-m_value);
}

std::shared_ptr<InternalObject> Number::unary_not () {
    return std::make_shared<Boolean>(!m_value);
}


std::shared_ptr<InternalObject> Number::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
}

std::string Number::get_string () const { return std::to_string(m_value); }
std::string Number::get_typename () const { return type_name; }
double Number::get_number () const { return m_value; }


std::shared_ptr<InternalObject> NumberFactory::create () const {
    return std::make_shared<Number>();
}

} /* namespace object */
} /* namespace mlang */