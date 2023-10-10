#pragma once

#include "mlang/exception.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace mlang {
namespace object {

class ObjectFactory;

class InternalObject {
public:
    InternalObject () = default;
    virtual ~InternalObject () = default;

    virtual std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) = 0;

    //virtual void construct (const std::vector<std::shared_ptr<InternalObject>>& params) = 0;
    //virtual void assign (const std::vector<std::shared_ptr<InternalObject>>& params) = 0;
    virtual void assign (const std::shared_ptr<InternalObject> param) = 0;
    //virtual void destruct () = 0;
    virtual std::string get_typename () const = 0;

    virtual bool is_true () const;
    virtual double get_number () const;
    virtual std::string get_string () const;

    virtual const ObjectFactory& get_factory () const = 0;

    /* += */
    virtual void operator_add_equal (const InternalObject* param);

    /* -= */
    virtual void operator_sub_equal (const InternalObject* param);

    /* *= */
    virtual void operator_mul_equal (const InternalObject* param);

    /* /= */
    virtual void operator_div_equal (const InternalObject* param);
    
    /* + */
    virtual std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param);
    
    /* - */
    virtual std::shared_ptr<InternalObject> operator_binary_sub (const InternalObject* param);
    
    /* * */
    virtual std::shared_ptr<InternalObject> operator_binary_mul (const InternalObject* param);
    
    /* / */
    virtual std::shared_ptr<InternalObject> operator_binary_div (const InternalObject* param);

    /* unary - */
    virtual std::shared_ptr<InternalObject> unary_minus ();

    /* unary ! */
    virtual std::shared_ptr<InternalObject> unary_not ();

    /* == */
    virtual std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param);

    /* != */
    virtual std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param);

    /* > */
    virtual std::shared_ptr<InternalObject> operator_greater (const InternalObject* param);

    /* < */
    virtual std::shared_ptr<InternalObject> operator_less (const InternalObject* param);

    /* >= */
    virtual std::shared_ptr<InternalObject> operator_greater_equal (const InternalObject* param);

    /* <= */
    virtual std::shared_ptr<InternalObject> operator_less_equal (const InternalObject* param);

    /* [] */
    virtual std::shared_ptr<InternalObject> operator_subscript (const InternalObject* param);
};

class ObjectFactory {
public:
    virtual std::shared_ptr<InternalObject> create () const = 0;
};

} /* namespace object */
} /* namespace mlang */