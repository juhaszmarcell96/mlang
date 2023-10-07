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
const std::string add_equal { "+=" };
const std::string sub_equal { "-=" };
const std::string mul_equal { "*=" };
const std::string div_equal { "/=" };
const std::string binary_add { "+" };
const std::string binary_sub { "-" };
const std::string binary_mul { "*" };
const std::string binary_div { "/" };
const std::string binary_equality { "==" };
const std::string binary_inequality { "!=" };
const std::string comparison_greater { ">" };
const std::string comparison_less { "<" };
const std::string comparison_greater_equal { ">=" };
const std::string comparison_less_equal { "<=" };
const std::string arr_indexing { "[]" };
const std::string unary_not { "!" };
const std::string unary_minus { "-a" };
const std::string boolean_operator { "()" };

} /* namespace operators */

class Object {
protected:
    bool m_lvalue { false };
    
    bool assert_true (bool val, const std::string& err_msg) {
        if (!val) { throw RuntimeError { err_msg }; }
    }

    template<typename T>
    T assert_cast (Object* obj, const std::string& type) {
        T ptr = dynamic_cast<T>(obj);
        if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
        return ptr;
    }

    template<typename T>
    T assert_cast (const Object* obj, const std::string& type) {
        T ptr = dynamic_cast<T>(obj);
        if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
        return ptr;
    }

    void assert_parameter (Object* obj, const std::string& type, const std::string& function) {
        if (!obj) { throw RuntimeError { "argument in member function '" + function + "' on object of type '" + type + "' is null" }; }
    }

    void assert_params(const std::vector<Object*>& params, std::size_t N, const std::string& type, const std::string& func) {
        if (params.size() != N) {
            throw RuntimeError { "invalid number of parameters for '" + type + "'::'" + func + "' function" };
        }
    }
public:
    Object () = delete;
    Object (bool lvalue) : m_lvalue(lvalue) {};
    virtual ~Object () {};
    virtual void call (const std::string& func, const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) = 0;

    virtual void construct (const std::vector<Object*>& params) = 0;
    virtual void assign (const std::vector<Object*>& params) = 0;
    virtual void assign (const Object* param) = 0;
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
};

class ObjectFactory {
public:
    virtual std::shared_ptr<Object> create () = 0;
};

} /* namespace mlang */