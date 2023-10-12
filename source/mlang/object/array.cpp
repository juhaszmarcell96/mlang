#include "mlang/object/object.hpp"
#include "mlang/object/array.hpp"
#include "mlang/object/boolean.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

Array::Array (const std::vector<Object>& arr) : m_arr(arr) {}

std::string Array::get_typename () const { return type_name; }

const ObjectFactory& Array::get_factory () const {
    static ArrayFactory factory{};
    return factory;
}

/* construct */
/*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override {
    if (params.size() == 0) { m_arr.clear(); return; }
    for (const auto param : params) {
        m_arr.push_back(Object{param});
    }
}*/
/* assign */
/*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override {
    m_arr = params;
}
*/
/* destruct */
/*void destruct () override {
    m_arr.clear();
}*/

void Array::assign (const std::shared_ptr<InternalObject> param) {
    const std::shared_ptr<Array> arr_ptr = assert_cast<Array>(param, type_name);
    m_arr = arr_ptr->m_arr;
}

std::shared_ptr<InternalObject> Array::operator_binary_add (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+");
    const std::shared_ptr<Array> arr_ptr = assert_cast<Array>(param, type_name);
    std::vector<Object> new_arr = m_arr;
    for (const auto elem : arr_ptr->m_arr) {
        new_arr.push_back(elem);
    }
    return std::make_shared<Array>(new_arr);
}

/* TODO append vs concatenate? */
void Array::operator_add_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "+=");
    m_arr.push_back(Object{param});
}

std::shared_ptr<InternalObject> Array::operator_comparison_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "==");
    const std::shared_ptr<Array> arr_ptr = assert_cast<Array>(param, type_name);
    const std::vector<Object>& other_arr = arr_ptr->m_arr;
    if (other_arr.size() != m_arr.size()) {
        return std::make_shared<Boolean>(false);
    }
    for (std::size_t i = 0; i < other_arr.size(); ++i) {
        if (other_arr[i] != m_arr[i]) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<InternalObject> Array::operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "!=");
    const std::shared_ptr<Array> arr_ptr = assert_cast<Array>(param, type_name);
    const std::vector<Object>& other_arr = arr_ptr->m_arr;
    if (other_arr.size() != m_arr.size()) {
        return std::make_shared<Boolean>(true);
    }
    for (std::size_t i = 0; i < other_arr.size(); ++i) {
        if (other_arr[i] != m_arr[i]) {
            return std::make_shared<Boolean>(true);
        }
    }
    return std::make_shared<Boolean>(false);
}

Object& Array::operator_subscript (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "[]");
    std::size_t index = static_cast<std::size_t>(param->get_number());
    return m_arr[index];
}

std::shared_ptr<InternalObject> Array::reverse () {
    std::vector<Object> reversed = m_arr;
    std::reverse(reversed.begin(), reversed.end());
    return std::make_shared<Array>(reversed);
}
/*
void concatenate (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "concatenate");
    const std::shared_ptr<Array> arr_ptr = assert_cast<Array>(param, type_name);
    for (const auto elem : arr_ptr->m_arr) {
        m_arr.push_back(elem);
    }
}
*/


std::shared_ptr<InternalObject> Array::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (func.compare("reverse") == 0) {
        return reverse();
    }
    else {
        throw RuntimeError { "string object has no " + func + " member function" };
    }
    return nullptr;
}

std::string Array::get_string () const {
    std::string str = type_name;
    str += " : { ";
    for (const auto& elem : m_arr) {
        str += elem.get_string();
        str += " ";
    }
    str += "}";
    return str;
}

std::shared_ptr<InternalObject> ArrayFactory::create () const {
    return std::make_shared<Array>();
}

} /* namespace object */
} /* namespace mlang */