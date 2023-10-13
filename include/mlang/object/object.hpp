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

struct WrapperObject {
    internal_obj_ptr obj;
};

/* wrapper around the internal object */
class Object {
protected:
    bool m_lvalue { false };
    std::shared_ptr<WrapperObject> m_object;
public:
    Object ();
    Object (bool lvalue);
    Object (internal_obj_ptr obj);
    Object (const ObjectFactory& factory);
    ~Object () = default;

    Object call (const std::string& func, const std::vector<Object>& params);
    Object access (const std::string& member);

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
    Object& operator_add_equal (const Object& rhs);

    /* -= */
    Object& operator_sub_equal (const Object& rhs);

    /* *= */
    Object& operator_mul_equal (const Object& rhs);

    /* /= */
    Object& operator_div_equal (const Object& rhs);
    
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

    /* postfix ++ */
    Object postfix_increment ();

    /* postfix -- */
    Object postfix_decrement ();

    /* prefix ++ */
    Object& prefix_increment ();

    /* prefix -- */
    Object& prefix_decrement ();

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
    Object& operator_subscript (const Object& param);

    /* && */
    Object operator_binary_and (const Object& rhs);

    /* || */
    Object operator_binary_or (const Object& rhs);
};

} /* namespace object */
} /* namespace mlang */