#pragma once

#include "mlang/object/internal_object.hpp"
#include "mlang/object/none.hpp"
#include "mlang/object/boolean.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace mlang {
namespace object {

/* wrapper around the internal object */
class Object {
protected:
    bool m_lvalue { false };
    std::shared_ptr<InternalObject> m_object;
public:
    Object ();
    Object (bool lvalue);
    Object (std::shared_ptr<InternalObject> obj);
    Object (const ObjectFactory& factory);
    ~Object () = default;

    Object call (const std::string& func, const std::vector<Object>& params);

    void construct (const std::vector<Object>& params);
    void assign (const Object& param);
    void destruct ();
    std::string get_typename () const;
    const ObjectFactory& get_factory () const;

    bool is_lvalue () const;
    void set_lvalue (bool lvalue);

    bool is_true () const;
    double get_number () const;
    std::string get_string () const;

    /* += */
    Object& operator_add_equal (const Object& rhs, const ObjectFactory& factory);

    /* -= */
    Object& operator_sub_equal (const Object& rhs, const ObjectFactory& factory);

    /* *= */
    Object& operator_mul_equal (const Object& rhs, const ObjectFactory& factory);

    /* /= */
    Object& operator_div_equal (const Object& rhs, const ObjectFactory& factory);
    
    /* + */
    Object operator_binary_add (const Object& rhs);
    Object operator+(const Object& rhs) const;
    
    /* - */
    Object operator_binary_sub (const Object& rhs);
    Object operator-(const Object& rhs) const;
    
    /* * */
    Object operator_binary_mul (const Object& rhs);
    Object operator*(const Object& rhs) const;
    
    /* / */
    Object operator_binary_div (const Object& rhs);
    Object operator/(const Object& rhs) const;

    /* unary - */
    Object unary_minus ();

    /* unary ! */
    Object unary_not ();

    /* == */
    friend bool operator==(const Object& lhs, const Object& rhs);
    Object operator_comparison_equal (const Object& rhs);

    /* != */
    Object operator_comparison_not_equal (const Object& rhs);

    /* > */
    Object operator_greater (const Object& rhs);

    /* < */
    Object operator_less (const Object& rhs);

    /* >= */
    Object operator_greater_equal (const Object& rhs);

    /* <= */
    Object operator_less_equal (const Object& rhs);

    /* [] */
    Object operator_subscript (const Object& param);
};

} /* namespace object */
} /* namespace mlang */