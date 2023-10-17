#include "mlang/object/string.hpp"
#include "mlang/object/number.hpp"
#include "mlang/object/assert.hpp"

#include <regex>

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

std::shared_ptr<InternalObject> String::is_empty () {
    return std::make_shared<Boolean>(m_value.empty());
}

std::shared_ptr<InternalObject> String::contains (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 1, type_name, "contains");
    assert_parameter(params[0], type_name, "contains");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
    if (m_value.find(str_ptr->get_string()) != std::string::npos) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<InternalObject> String::contains_regex (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 1, type_name, "contains_regex");
    assert_parameter(params[0], type_name, "contains_regex");
    const std::shared_ptr<String> str_ptr = assert_cast<String>(params[0], type_name);
    std::regex m_regex (str_ptr->get_string());
    if (std::regex_search(m_value, m_regex)) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<InternalObject> String::regex_replace (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 2, type_name, "regex_replace");
    assert_parameter(params[0], type_name, "regex_replace");
    assert_parameter(params[1], type_name, "regex_replace");
    const std::shared_ptr<String> param_regex = assert_cast<String>(params[0], type_name);
    const std::shared_ptr<String> param_replace = assert_cast<String>(params[1], type_name);
    std::regex m_regex (param_regex->get_string());
    return std::make_shared<String>(std::regex_replace(m_value, m_regex, param_replace->get_string()));
}

std::shared_ptr<InternalObject> String::regex_find (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 2, type_name, "regex_find");
    assert_parameter(params[0], type_name, "regex_find");
    assert_parameter(params[1], type_name, "regex_find");
    const std::shared_ptr<String> param_regex = assert_cast<String>(params[0], type_name);
    const std::shared_ptr<String> param_format = assert_cast<String>(params[1], type_name);
    std::regex m_regex { param_regex->get_string() };
    std::string m_format { param_format->get_string() };
    if (std::regex_search(m_value, m_regex)) {
        std::sregex_iterator match_begin = std::sregex_iterator(m_value.begin(), m_value.end(), m_regex);
        std::smatch first_match = *match_begin;
        std::string value = std::regex_replace (first_match.str(), m_regex, m_format, std::regex_constants::format_no_copy);
        return std::make_shared<String>(value);
    }
    return std::make_shared<String>("");
}


std::shared_ptr<InternalObject> String::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (func.compare("reverse") == 0) {
        return reverse();
    }
    else if (func.compare("length") == 0) {
        return length();
    }
    else if (func.compare("is_empty") == 0) {
        return is_empty();
    }
    else if (func.compare("contains") == 0) {
        return contains(params);
    }
    else if (func.compare("contains_regex") == 0) {
        return contains_regex(params);
    }
    else if (func.compare("regex_replace") == 0) {
        return regex_replace(params);
    }
    else if (func.compare("regex_find") == 0) {
        return regex_find(params);
    }
    else {
        throw RuntimeError { "string object has no " + func + " member function" };
    }
    return nullptr;
}

std::shared_ptr<InternalObject> String::access (const std::string& member) {
    throw RuntimeError { "object of type '" + type_name + "' has no '" + member + "' member" };
}

std::string String::get_string () const { return m_value; }
std::string String::get_typename () const { return type_name; }


std::shared_ptr<InternalObject> StringFactory::create () const {
    return std::make_shared<String>();
}

} /* namespace object */
} /* namespace mlang */