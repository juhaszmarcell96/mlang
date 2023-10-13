#pragma once

#include <string>
#include <exception>

namespace mlang {

class LangException : public std::exception { };

class SyntaxError : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    SyntaxError () : m_message("syntax error") {}
    SyntaxError (const std::string& message) : m_message(message) {}
    SyntaxError (const std::string& message, std::size_t line, std::size_t col) {
        m_message = "syntax error : ";
        m_message += message;
        m_message += " (line " + std::to_string(line) + " , position " + std::to_string(col) + ")";
    }
};

class RuntimeError : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    RuntimeError () : m_message("runtime error") {}
    RuntimeError (const std::string& message) : m_message(message) {}
};

} /* namespace mlang */