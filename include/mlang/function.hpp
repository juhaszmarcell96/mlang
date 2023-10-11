#pragma once

#include <memory>
#include <string>
#include <vector>

#include "mlang/exception.hpp"

namespace mlang {

namespace ast {
    // forward declare FunctionDeclNode
    class FunctionDeclNode;
} /* namespace ast */

class Function {
private:
    const ast::FunctionDeclNode* m_function { nullptr };
public:
    Function (const ast::FunctionDeclNode* function) : m_function(function) {}
    ~Function () = default;

    const ast::FunctionDeclNode* get () { return m_function; }
};

} /* namespace mlang */