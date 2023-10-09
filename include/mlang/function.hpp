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
    const FunctionDeclNode* m_function { nullptr };
public:
    Function (const FunctionDeclNode* function) : m_function(function) {}
    ~Function () = default;

    const FunctionDeclNode* get () { return m_function; }
};

} /* namespace mlang */