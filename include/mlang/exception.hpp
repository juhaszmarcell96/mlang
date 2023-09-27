#pragma once

#include <string>
#include <exception>

namespace mlang {

class bad_value_type : public std::exception {
public:
    const char* what () const noexcept override {
        return "bad type";
    }
};

class bad_operation : public std::exception {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    bad_operation () : m_message("bad operation") {}
    bad_operation (const std::string& message) : m_message(message) {}
};

class syntax_error : public std::exception {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    syntax_error () : m_message("syntax error") {}
    syntax_error (const std::string& message, std::size_t line, std::size_t col) {
        m_message = "syntax error : ";
        m_message += message;
        m_message += " (line " + std::to_string(line) + " , position " + std::to_string(col) + ")";
    }
};

class semantics_error : public std::exception {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    semantics_error () : m_message("semantics error") {}
    semantics_error (const std::string& message) : m_message(message) {}
    semantics_error (const std::string& message, std::size_t line, std::size_t col) {
        m_message = "semantics error : ";
        m_message += message;
        m_message += " (line " + std::to_string(line) + " , position " + std::to_string(col) + ")";
    }
};

class unexpected_error : public std::exception {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    unexpected_error () : m_message("unexpected error") {}
    unexpected_error (const std::string& message) : m_message(message) {}
};

} /* namespace mlang */