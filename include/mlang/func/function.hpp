#pragma once

#include <vector>

#include "mlang/object/object.hpp"
#include "mlang/script/environment.hpp"

namespace mlang {
namespace func {

class Function {
public:
    virtual object::Object call (script::EnvStack& env, std::vector<object::Object>& params) const = 0;
};

} /* namespace func */
} /* namespace mlang */