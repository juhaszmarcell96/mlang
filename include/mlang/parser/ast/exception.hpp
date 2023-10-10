#pragma once

#include <string>
#include <exception>

#include "mlang/object/object.hpp"

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
    Object m_val;
public:
    Return (const Object& val) : m_val(val) {}
    const char* what () const noexcept override {
        return "return";
    }

    const Object& get_value () const { return m_val; }
};

class Exit : public std::exception {
private:
    Object m_val;
public:
    Exit (const Object& val) : m_val(val) {}
    const char* what () const noexcept override {
        return "exit";
    }

    const Object& get_value () const { return m_val; }
};

} /* namespace mlang */