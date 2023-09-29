#pragma once

#include <memory>
#include <string>
#include <vector>

#include "exception.hpp"

namespace mlang {

enum class value_types {
    none,
    string,
    number,
    array,
    boolean
};

static inline std::string to_string (value_types type) {
    switch (type) {
        case value_types::none : {
            return "none";
        }
        case value_types::string : {
            return "string";
        }
        case value_types::number : {
            return "number";
        }
        case value_types::array : {
            return "array";
        }
        case value_types::boolean : {
            return "boolean";
        }
        default : {
            return "error";
        }
    }
}

class Value;

typedef double Number;
typedef bool Boolean;
typedef std::string String;
typedef std::vector<Value> Array;

class Value {
private:
    value_types m_type = value_types::none;
    void* m_value = nullptr;

    static void assert_type(value_types t1, value_types t2) {
        if (t1 != t2) {
            throw bad_value_type{};
        }
    }

    static void assert_not_type(value_types t1, value_types t2) {
        if (t1 == t2) {
            throw bad_value_type{};
        }
    }

    inline Number*  as_number  () const { return static_cast<Number*>(m_value); }
    inline String*  as_string  () const { return static_cast<String*>(m_value); }
    inline Array*   as_array   () const { return static_cast<Array*>(m_value);  }
    inline Boolean* as_boolean () const { return static_cast<Boolean*>(m_value);  }
public:
    /* constructor */
    Value() = default;
    Value(value_types type) : m_type(type) {
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { m_value = new Number; *as_number() = 0; break; }
            case value_types::string:  { m_value = new String; break; }
            case value_types::array:   { m_value = new Array;  break; }
            case value_types::boolean: { m_value = new Boolean; *as_boolean() = false; break; }
            default: { throw bad_value_type{}; }
        }
    }
    explicit Value (Number num)        { m_value = new Number;  *as_number()  = num; m_type = value_types::number; }
    explicit Value (int num)           { m_value = new Number;  *as_number()  = num; m_type = value_types::number; }
    explicit Value (const String& str) { m_value = new String;  *as_string()  = str; m_type = value_types::string; }
    explicit Value (const char* str)   { m_value = new String;  *as_string()  = str; m_type = value_types::string; }
    explicit Value (const Array& arr)  { m_value = new Array;   *as_array()   = arr; m_type = value_types::array; }
    explicit Value (Boolean boo)       { m_value = new Boolean; *as_boolean() = boo; m_type = value_types::boolean; }

    /* copy constructor */
    Value(const Value& other) {
        m_type = other.m_type;
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { m_value = new Number;  *as_number()  = other.get_number();  break; }
            case value_types::string:  { m_value = new String;  *as_string()  = other.get_string();  break; }
            case value_types::array:   { m_value = new Array;   *as_array()   = other.get_array();   break; }
            case value_types::boolean: { m_value = new Boolean; *as_boolean() = other.get_boolean(); break; }
            default: { throw bad_value_type{}; }
        }
    }

    /* copy assignment */
    Value& operator=(const Value& rhs) {
        if (this == &rhs) return *this;
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { delete as_number();  break; }
            case value_types::string:  { delete as_string();  break; }
            case value_types::array:   { delete as_array();   break; }
            case value_types::boolean: { delete as_boolean(); break; }
            default: { throw bad_value_type{}; }
        }
        m_type = rhs.m_type;
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { m_value = new Number;  *as_number()  = rhs.get_number();  break; }
            case value_types::string:  { m_value = new String;  *as_string()  = rhs.get_string();  break; }
            case value_types::array:   { m_value = new Array;   *as_array()   = rhs.get_array();   break; }
            case value_types::boolean: { m_value = new Boolean; *as_boolean() = rhs.get_boolean(); break; }
            default: { throw bad_value_type{}; }
        }
        return *this;
    }

    /* move constructor */
    Value (Value&& other) noexcept {
        m_type = other.m_type;
        other.m_type = value_types::none;
        m_value = other.m_value;
        other.m_value = nullptr;
    }

    /* move assignment */
    Value& operator=(Value&& rhs) noexcept {
        if (this == &rhs) return *this;
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { delete as_number();  break; }
            case value_types::string:  { delete as_string();  break; }
            case value_types::array:   { delete as_array();   break; }
            case value_types::boolean: { delete as_boolean(); break; }
            default: { break; }
        }
        m_type = rhs.m_type;
        rhs.m_type = value_types::none;
        m_value = rhs.m_value;
        rhs.m_value = nullptr;
        return *this;
    }

    /* destructor */
    ~Value() {
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { delete as_number();  break; }
            case value_types::string:  { delete as_string();  break; }
            case value_types::array:   { delete as_array();   break; }
            case value_types::boolean: { delete as_boolean(); break; }
            default: { break; }
        }
     }


    value_types get_type () const { return m_type; }
    const Number&  get_number  () const { assert_type(m_type, value_types::number);  return *as_number();  }
    const String&  get_string  () const { assert_type(m_type, value_types::string);  return *as_string();  }
    const Array&   get_array   () const { assert_type(m_type, value_types::array);  return *as_array();   }
    const Boolean& get_boolean () const { assert_type(m_type, value_types::boolean); return *as_boolean(); }

    /* prefix increment */
    Value& operator++() {
        switch (m_type) {
            case value_types::none:    { throw bad_operation ("value of type 'none' cannot be incremented"); }
            case value_types::number:  { *as_number() += 1; return *this; }
            case value_types::string:  { throw bad_operation ("value of type 'string' cannot be incremented"); }
            case value_types::array:   { throw bad_operation ("value of type 'array' cannot be incremented"); }
            case value_types::boolean: { throw bad_operation ("value of type 'boolean' cannot be incremented"); }
            default: { throw bad_value_type{}; }
        }
    }

    /* postfix increment */
    Value operator++(int) {
        switch (m_type) {
            case value_types::none:    { throw bad_operation ("value of type 'none' cannot be incremented"); }
            case value_types::number:  { Value old = *this; *as_number() += 1; return old; }
            case value_types::string:  { throw bad_operation ("value of type 'string' cannot be incremented"); }
            case value_types::array:   { throw bad_operation ("value of type 'array' cannot be incremented"); }
            case value_types::boolean: { throw bad_operation ("value of type 'boolean' cannot be incremented"); }
            default: { throw bad_value_type{}; }
        }
    }

    /* TODO : prefix decrement */
    /* TODO : postfix decrement */

    /* array subscript operator */
    Value& operator[](std::size_t index) {
        switch (m_type) {
            case value_types::none:    { throw bad_operation ("value of type 'none' has no array subscript operator"); }
            case value_types::number:  { throw bad_operation ("value of type 'number' has no array subscript operator"); }
            case value_types::string:  { throw bad_operation ("value of type 'string' has no array subscript operator"); }
            case value_types::array:   { return (*as_array())[index]; }
            case value_types::boolean: { throw bad_operation ("value of type 'boolean' has no array subscript operator"); }
            default: { throw bad_value_type{}; }
        }
    }

    const Value& operator[](std::size_t index) const {
        switch (m_type) {
            case value_types::none:    { throw bad_operation ("value of type 'none' has no array subscript operator"); }
            case value_types::number:  { throw bad_operation ("value of type 'number' has no array subscript operator"); }
            case value_types::string:  { throw bad_operation ("value of type 'string' has no array subscript operator"); }
            case value_types::array:   { return (*as_array())[index]; }
            case value_types::boolean: { throw bad_operation ("value of type 'boolean' has no array subscript operator"); }
            default: { throw bad_value_type{}; }
        }
    }

    /* append */
    void append(const Value& var) {
        switch (m_type) {
            case value_types::none:   { throw bad_operation ("value of type 'none' has not append operation defined"); }
            case value_types::number: { throw bad_operation ("value of type 'number' has not append operation defined"); }
            case value_types::string: {
                if (var.m_type == value_types::number) {
                    *as_string() += std::to_string(*(var.as_number()));
                }
                else if (var.m_type == value_types::string) {
                    *as_string() += *(var.as_string());
                }
                else {
                    throw bad_value_type{};
                }
                break;
            }
            case value_types::array:   { return (*as_array()).push_back(var); }
            case value_types::boolean: { throw bad_operation ("value of type 'boolean' has not append operation defined"); }
            default: { throw bad_value_type{}; }
        }
    }

    /* clear */
    void clear() {
        switch (m_type) {
            case value_types::none:    { break; }
            case value_types::number:  { (*as_number()) = 0; break; }
            case value_types::string:  { as_string()->clear(); break; }
            case value_types::array:   { as_array()->clear(); break; }
            case value_types::boolean: { (*as_boolean()) = false; break; }
            default: { throw bad_value_type{}; }
        }
    }

    /* ostream */
    friend std::ostream &operator<<(std::ostream &os, const Value& var) {
        switch (var.m_type) {
            case value_types::none:    { throw bad_operation ("value of type 'none' cannot be printed"); }
            case value_types::number:  { os << *(var.as_number()); break; }
            case value_types::string:  { os << *(var.as_string()); break; }
            case value_types::array:   { for (const Value& v : *(var.as_array())) { os << v << " "; }; break; }
            case value_types::boolean: { os << (*(var.as_boolean()) ? "true" : "false"); break; }
            default: { throw bad_value_type{}; }
        }
        return os;
    }
};

} /* namespace mlang */