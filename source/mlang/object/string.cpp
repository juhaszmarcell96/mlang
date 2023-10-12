#include "mlang/object/string.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

String::String (const std::string& value) : m_value(value) {}

const std::string& String::get () const { return m_value; }

const ObjectFactory& String::get_factory () const {
    static StringFactory factory{};
    return factory;
}

/* construct */
void String::construct (const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (params.size() == 0) { m_value = ""; }
    assert_params(params, 1, type_name, "constructor");
    assert_parameter(params[0], type_name, "constructor");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
    m_value = str_ptr->get();
}
/* assign */
/*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 1, type_name, operators::construct);
    assert_parameter(params[0], type_name, operators::construct);
    const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
    m_value = str_ptr->get();
}*/
/**/
/* destruct */
/*void destruct () {
    m_value = "";
}*/
void String::assign (const std::shared_ptr<InternalObject> param) {
    const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
    m_value = str_ptr->get();
}

std::shared_ptr<InternalObject> String::operator_binary_add (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
    return std::make_shared<String>(m_value + str_ptr->get());
}

void String::operator_add_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+=");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
    m_value += str_ptr->get();
}

std::shared_ptr<InternalObject> String::operator_comparison_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "==");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
    return std::make_shared<Boolean>(m_value == str_ptr->get());
}

std::shared_ptr<InternalObject> String::operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "!=");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(param, type_name);
    return std::make_shared<Boolean>(m_value != str_ptr->get());
}

std::shared_ptr<InternalObject> String::reverse () {
    std::string reversed = m_value;
    std::reverse(reversed.begin(), reversed.end());
    return std::make_shared<String>(reversed);
}

std::shared_ptr<InternalObject> String::length () {
    return std::make_shared<Number>(m_value.length());
}


std::shared_ptr<InternalObject> String::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (func.compare("reverse") == 0) {
        return reverse();
    }
    else if (func.compare("length") == 0) {
        return length();
    }
    else {
        throw RuntimeError { "string object has no " + func + " member function" };
    }
    return nullptr;
}

std::string String::get_string () const { return m_value; }
std::string String::get_typename () const { return type_name; }


std::shared_ptr<InternalObject> StringFactory::create () const {
    return std::make_shared<String>();
}

} /* namespace object */
} /* namespace mlang */