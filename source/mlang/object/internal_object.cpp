#pragma once

#include "mlang/exception.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace mlang {

class InternalObject {
protected:
    
    bool assert_true (bool val, const std::string& err_msg) {
        if (!val) { throw RuntimeError { err_msg }; }
    }

    template<typename T>
    const std::shared_ptr<T> assert_cast (const std::shared_ptr<InternalObject> obj, const std::string& type) {
        const std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(obj);
        if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
        return ptr;
    }

    template<typename T>
    T assert_cast (const InternalObject* obj, const std::string& type) {
        T ptr = dynamic_cast<T>(obj);
        if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
        return ptr;
    }

    void assert_parameter (const std::shared_ptr<InternalObject> obj, const std::string& type, const std::string& function) {
        if (!obj) { throw RuntimeError { "argument in member function '" + function + "' on object of type '" + type + "' is null" }; }
    }

    void assert_parameter (const InternalObject* obj, const std::string& type, const std::string& function) {
        if (!obj) { throw RuntimeError { "argument in member function '" + function + "' on object of type '" + type + "' is null" }; }
    }

    void assert_params(const std::vector<std::shared_ptr<InternalObject>>& params, std::size_t N, const std::string& type, const std::string& func) {
        if (params.size() != N) {
            throw RuntimeError { "invalid number of parameters for '" + type + "'::'" + func + "' function" };
        }
    }
public:
    InternalObject () = default;
    virtual ~InternalObject () = default;

    virtual std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) = 0;

    //virtual void construct (const std::vector<std::shared_ptr<InternalObject>>& params) = 0;
    //virtual void assign (const std::vector<std::shared_ptr<InternalObject>>& params) = 0;
    //virtual void assign (const std::shared_ptr<InternalObject> param) = 0;
    //virtual void destruct () = 0;
    virtual std::string get_typename () const = 0;

    virtual bool is_true () const {
        throw RuntimeError { "object of type '" + get_typename() + "' cannot be evaluated as boolean" };
    }
    virtual double get_number () const {
        throw RuntimeError { "object of type '" + get_typename() + "' cannot be interpreted as number" };
    }
    virtual std::string get_string () const {
        throw RuntimeError { "object of type '" + get_typename() + "' cannot be interpreted as string" };
    }

    /* += */
    virtual void operator_add_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '+=' operator" };
    }

    /* -= */
    virtual void operator_sub_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '-=' operator" };
    }

    /* *= */
    virtual void operator_mul_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '*=' operator" };
    }

    /* /= */
    virtual void operator_div_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '/=' operator" };
    }
    
    /* + */
    virtual std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '+' operator" };
    }
    
    /* - */
    virtual std::shared_ptr<InternalObject> operator_binary_sub (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '-' operator" };
    }
    
    /* * */
    virtual std::shared_ptr<InternalObject> operator_binary_mul (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '*' operator" };
    }
    
    /* / */
    virtual std::shared_ptr<InternalObject> operator_binary_div (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '/' operator" };
    }

    /* unary - */
    virtual std::shared_ptr<InternalObject> unary_minus () {
        throw RuntimeError { "object of type '" + get_typename() + "' has no 'unary -' operator" };
    }

    /* unary ! */
    virtual std::shared_ptr<InternalObject> unary_not () {
        throw RuntimeError { "object of type '" + get_typename() + "' has no 'unary !' operator" };
    }

    /* == */
    virtual std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '==' operator" };
    }

    /* != */
    virtual std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '!=' operator" };
    }

    /* > */
    virtual std::shared_ptr<InternalObject> operator_greater (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '>' operator" };
    }

    /* < */
    virtual std::shared_ptr<InternalObject> operator_less (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '<' operator" };
    }

    /* >= */
    virtual std::shared_ptr<InternalObject> operator_greater_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '>=' operator" };
    }

    /* <= */
    virtual std::shared_ptr<InternalObject> operator_less_equal (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '<=' operator" };
    }

    /* [] */
    virtual std::shared_ptr<InternalObject> operator_subscript (const InternalObject* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '[]' operator" };
    }
};

class ObjectFactory {
public:
    virtual std::shared_ptr<InternalObject> create () const = 0;
};

} /* namespace mlang */