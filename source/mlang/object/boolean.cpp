#include "mlang/object/boolean.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

Boolean::Boolean (bool value) : m_value(value) {}

const bool Boolean::get () const { return m_value; }

const ObjectFactory& Boolean::get_factory () const {
    static BooleanFactory factory{};
    return factory;
}

/* construct */
/*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) {
    if (params.size() == 0) { m_value = false; }
    assert_params(params, 1, type_name, "constructor");
    assert_parameter(params[0], type_name, "constructor");
    m_value = params[0]->is_true();
}*/
/* assign */
/*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) {
    assert_params(params, 1, type_name, operators::assign);
    assert_parameter(params[0], type_name, operators::assign);
    const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(params[0], type_name);
    m_value = bool_ptr->get();
}*/
/* destruct */
/*void destruct () {
    m_value = false;
}*/
void Boolean::assign (const std::shared_ptr<InternalObject> param) {
    const std::shared_ptr<Boolean> bool_ptr = assert_cast<Boolean>(param, type_name);
    m_value = bool_ptr->m_value;
}

bool Boolean::is_true () const {
    return m_value;
}

std::shared_ptr<InternalObject> Boolean::operator_comparison_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "==");
    return std::make_shared<Boolean>(m_value == param->is_true());
}

std::shared_ptr<InternalObject> Boolean::operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) {
    assert_parameter(param, type_name, "!=");
    return std::make_shared<Boolean>(m_value != param->is_true());
}

std::shared_ptr<InternalObject> Boolean::unary_not () {
    return std::make_shared<Boolean>(!m_value);
}


std::shared_ptr<InternalObject> Boolean::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    throw RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
    return nullptr;
}

std::string Boolean::get_string () const { return (m_value ? "true" : "false"); }
std::string Boolean::get_typename () const { return type_name; }
double Boolean::get_number () const { return (m_value ? 1 : 0); }


std::shared_ptr<InternalObject> BooleanFactory::create () const {
    return std::make_shared<Boolean>();
}

} /* namespace object */
} /* namespace mlang */