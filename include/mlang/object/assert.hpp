#pragma once

#pragma once

#include "mlang/exception.hpp"
#include "mlang/object/internal_object.hpp"

#include <string>
#include <memory>
#include <vector>

namespace mlang {
namespace object {

inline bool assert_true (bool val, const std::string& err_msg) {
    if (!val) { throw RuntimeError { err_msg }; }
}

template<typename T>
const std::shared_ptr<T> assert_cast (const std::shared_ptr<InternalObject> obj, const std::string& type) {
    const std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(obj);
    if (!ptr) { throw RuntimeError { "parameter must be of type '" + type + "'" }; }
    return ptr;
}

inline void assert_parameter (const std::shared_ptr<InternalObject> obj, const std::string& type, const std::string& function) {
    if (!obj) { throw RuntimeError { "argument in member function '" + function + "' on object of type '" + type + "' is null" }; }
}

inline void assert_params(const std::vector<std::shared_ptr<InternalObject>>& params, std::size_t N, const std::string& type, const std::string& func) {
    if (params.size() != N) {
        throw RuntimeError { "invalid number of parameters for '" + type + "'::'" + func + "' function" };
    }
}

} /* namespace object */
} /* namespace mlang */