#pragma once

#include <memory>
#include <string>
#include <vector>

#include "mlang/exception.hpp"

namespace mlang {

// forward declare FunctionDeclNode
class FunctionDeclNode;

class Function {
private:
    FunctionDeclNode* m_function { nullptr };
public:
    Function (FunctionDeclNode* function) : m_function(function) {}
    ~Function () = default;

    FunctionDeclNode* get () { return m_function; }
};

} /* namespace mlang */