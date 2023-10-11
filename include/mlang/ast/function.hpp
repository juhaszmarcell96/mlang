#pragma once

#include <vector>

#include "mlang/object/object.hpp"

namespace mlang {
namespace ast {

class Function {
public:
    virtual object::Object call (std::vector<object::Object>& params) const = 0;
};

} /* namespace ast */
} /* namespace mlang */