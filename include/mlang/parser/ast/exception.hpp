#pragma once

#include <string>
#include <exception>

#include "mlang/value.hpp"

namespace mlang {

class Break : public std::exception {
public:
    const char* what () const noexcept override {
        return "break";
    }
};

class Continue : public std::exception {
public:
    const char* what () const noexcept override {
        return "continue";
    }
};

/* return should probably hold the return value */
class Return : public std::exception {
private:
    Value m_val;
public:
    Return (const Value& val) : m_val(val) {}
    const char* what () const noexcept override {
        return "return";
    }

    const Value& get_value () const { return m_val; }
};

} /* namespace mlang */