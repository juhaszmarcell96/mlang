#pragma once

#include <string>
#include <vector>

#include "mlang/script/token.hpp"
#include "mlang/script/environment.hpp"

#define DEBUG_SCRIPT 0

#if DEBUG_SCRIPT == 1
#include <iostream>
#endif

namespace mlang {
namespace script {

class Script {
private:
    std::vector<Token> m_tokens;

    void debug(const std::string& debug_message);
public:
    Script () = delete;
    Script (const std::string& script);
    ~Script () = default;

    
    const std::vector<Token>& get_tokens () const;

    int execute (EnvStack& env);
};

} /* namespace script */
} /* namespace mlang */