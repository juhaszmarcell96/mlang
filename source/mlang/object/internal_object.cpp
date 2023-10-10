#include "mlang/object/internal_object.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

bool InternalObject::is_true () const {
    throw RuntimeError { "object of type '" + get_typename() + "' cannot be evaluated as boolean" };
}
double InternalObject::get_number () const {
    throw RuntimeError { "object of type '" + get_typename() + "' cannot be interpreted as number" };
}
std::string InternalObject::get_string () const {
    throw RuntimeError { "object of type '" + get_typename() + "' cannot be interpreted as string" };
}

/* += */
void InternalObject::operator_add_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '+=' operator" };
}

/* -= */
void InternalObject::operator_sub_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '-=' operator" };
}

/* *= */
void InternalObject::operator_mul_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '*=' operator" };
}

/* /= */
void InternalObject::operator_div_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '/=' operator" };
}

/* + */
std::shared_ptr<InternalObject> InternalObject::operator_binary_add (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '+' operator" };
}

/* - */
std::shared_ptr<InternalObject> InternalObject::operator_binary_sub (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '-' operator" };
}

/* * */
std::shared_ptr<InternalObject> InternalObject::operator_binary_mul (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '*' operator" };
}

/* / */
std::shared_ptr<InternalObject> InternalObject::operator_binary_div (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '/' operator" };
}

/* unary - */
std::shared_ptr<InternalObject> InternalObject::unary_minus () {
    throw RuntimeError { "object of type '" + get_typename() + "' has no 'unary -' operator" };
}

/* unary ! */
std::shared_ptr<InternalObject> InternalObject::unary_not () {
    throw RuntimeError { "object of type '" + get_typename() + "' has no 'unary !' operator" };
}

/* == */
std::shared_ptr<InternalObject> InternalObject::operator_comparison_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '==' operator" };
}

/* != */
std::shared_ptr<InternalObject> InternalObject::operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '!=' operator" };
}

/* > */
std::shared_ptr<InternalObject> InternalObject::operator_greater (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '>' operator" };
}

/* < */
std::shared_ptr<InternalObject> InternalObject::operator_less (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '<' operator" };
}

/* >= */
std::shared_ptr<InternalObject> InternalObject::operator_greater_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '>=' operator" };
}

/* <= */
std::shared_ptr<InternalObject> InternalObject::operator_less_equal (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '<=' operator" };
}

/* [] */
std::shared_ptr<InternalObject> InternalObject::operator_subscript (const std::shared_ptr<InternalObject> param) {
    throw RuntimeError { "object of type '" + get_typename() + "' has no '[]' operator" };
}

} /* namespace object */
} /* namespace mlang */