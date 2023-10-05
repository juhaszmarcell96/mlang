#pragma once

#include <string>
#include <exception>

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
public:
    const char* what () const noexcept override {
        return "return";
    }
};

} /* namespace mlang */