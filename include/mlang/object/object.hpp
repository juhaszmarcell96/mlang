#pragma once

#include "mlang/exception.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace mlang {

namespace operators {

const std::string construct { "new" };
const std::string destruct { "del" };
const std::string assign { "=" };
const std::string arr_indexing { "[]" };

} /* namespace operators */

class Object {
protected:
    bool m_lvalue { false };
    
    bool assert_true (bool val, const std::string& err_msg) {
        if (!val) { throw RuntimeError { err_msg }; }
    }

    //template<typename T>
    //std::shared_ptr<T> assert_cast (std::shared_ptr<Object> obj, const std::string& type) {
    //    std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(obj);
    //    if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
    //    return ptr;
    //}

    template<typename T>
    const std::shared_ptr<T> assert_cast (const std::shared_ptr<Object> obj, const std::string& type) {
        const std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(obj);
        if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
        return ptr;
    }

    //template<typename T>
    //T assert_cast (Object* obj, const std::string& type) {
    //    T ptr = dynamic_cast<T>(obj);
    //    if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
    //    return ptr;
    //}

    template<typename T>
    T assert_cast (const Object* obj, const std::string& type) {
        T ptr = dynamic_cast<T>(obj);
        if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
        return ptr;
    }

    void assert_parameter (const std::shared_ptr<Object> obj, const std::string& type, const std::string& function) {
        if (!obj) { throw RuntimeError { "argument in member function '" + function + "' on object of type '" + type + "' is null" }; }
    }

    void assert_parameter (const Object* obj, const std::string& type, const std::string& function) {
        if (!obj) { throw RuntimeError { "argument in member function '" + function + "' on object of type '" + type + "' is null" }; }
    }

    void assert_params(const std::vector<std::shared_ptr<Object>>& params, std::size_t N, const std::string& type, const std::string& func) {
        if (params.size() != N) {
            throw RuntimeError { "invalid number of parameters for '" + type + "'::'" + func + "' function" };
        }
    }
public:
    Object () = delete;
    Object (bool lvalue) : m_lvalue(lvalue) {};
    virtual ~Object () {};
    virtual void call (const std::string& func, const std::vector<std::shared_ptr<Object>>& params, std::shared_ptr<Object>& ret_val) = 0;

    virtual void construct (const std::vector<std::shared_ptr<Object>>& params) = 0;
    virtual void assign (const std::vector<std::shared_ptr<Object>>& params) = 0;
    virtual void assign (const std::shared_ptr<Object> param) = 0;
    virtual void destruct () = 0;
    virtual std::string get_typename () const = 0;

    bool is_lvalue () const { return m_lvalue; }
    void set_lvalue (bool lvalue) { m_lvalue = lvalue; }

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
    virtual void operator_add_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '+=' operator" };
    }

    /* -= */
    virtual void operator_sub_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '-=' operator" };
    }

    /* *= */
    virtual void operator_mul_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '*=' operator" };
    }

    /* /= */
    virtual void operator_div_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '/=' operator" };
    }
    
    /* + */
    virtual std::shared_ptr<Object> operator_binary_add (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '+' operator" };
    }
    
    /* - */
    virtual std::shared_ptr<Object> operator_binary_sub (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '-' operator" };
    }
    
    /* * */
    virtual std::shared_ptr<Object> operator_binary_mul (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '*' operator" };
    }
    
    /* / */
    virtual std::shared_ptr<Object> operator_binary_div (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '/' operator" };
    }

    /* unary - */
    virtual std::shared_ptr<Object> unary_minus () {
        throw RuntimeError { "object of type '" + get_typename() + "' has no 'unary -' operator" };
    }

    /* unary ! */
    virtual std::shared_ptr<Object> unary_not () {
        throw RuntimeError { "object of type '" + get_typename() + "' has no 'unary !' operator" };
    }

    /* == */
    virtual std::shared_ptr<Object> operator_comparison_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '==' operator" };
    }

    /* != */
    virtual std::shared_ptr<Object> operator_comparison_not_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '!=' operator" };
    }

    /* > */
    virtual std::shared_ptr<Object> operator_greater (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '>' operator" };
    }

    /* < */
    virtual std::shared_ptr<Object> operator_less (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '<' operator" };
    }

    /* >= */
    virtual std::shared_ptr<Object> operator_greater_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '>=' operator" };
    }

    /* <= */
    virtual std::shared_ptr<Object> operator_less_equal (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '<=' operator" };
    }

    /* [] */
    virtual std::shared_ptr<Object> operator_array_index (const Object* param) {
        throw RuntimeError { "object of type '" + get_typename() + "' has no '[]' operator" };
    }
};

class ObjectFactory {
public:
    virtual std::shared_ptr<Object> create () = 0;
};

} /* namespace mlang */