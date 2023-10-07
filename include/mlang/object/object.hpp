#pragma once

#include "mlang/exception.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace mlang {

class Object {
protected:
    bool m_lvalue { false };
public:
    Object () = delete;
    Object (bool lvalue) : m_lvalue(lvalue) {};
    virtual ~Object () {};
    virtual void call (const std::string& func, const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) = 0;

    virtual void construct (const std::vector<Object*>& params) = 0;
    virtual Object* assign (const std::vector<Object*>& params) = 0;
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