#pragma once

#include <string>
#include <exception>

namespace mlang {

class LangException : public std::exception { };

class bad_value_type : public LangException {
public:
    const char* what () const noexcept override {
        return "bad type";
    }
};

class bad_operation : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    bad_operation () : m_message("bad operation") {}
    bad_operation (const std::string& message) : m_message(message) {}
};

class syntax_error : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    syntax_error () : m_message("syntax error") {}
    syntax_error (const std::string& message) : m_message(message) {}
    syntax_error (const std::string& message, std::size_t line, std::size_t col) {
        m_message = "syntax error : ";
        m_message += message;
        m_message += " (line " + std::to_string(line) + " , position " + std::to_string(col) + ")";
    }
};

class semantics_error : public LangException {
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

class unexpected_error : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    unexpected_error () : m_message("unexpected error") {}
    unexpected_error (const std::string& message) : m_message(message) {}
};

class redeclaration_error : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    redeclaration_error () : m_message("variable already exists") {}
    redeclaration_error (const std::string& var_name) {
        m_message = "variable '" + var_name + "' already exists -> redeclaration";
    }
};

class bad_type_error : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    bad_type_error () : m_message("invalid type") {}
    bad_type_error (const std::string& type_name) {
        m_message = "type name '" + type_name + "' is invalid";
    }
};

class undefined_var_error : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    undefined_var_error () : m_message("variable undefined") {}
    undefined_var_error (const std::string& var_name) {
        m_message = "variable '" + var_name + "' is undefined";
    }
};

class incompatible_type_error : public LangException {
private:
    std::string m_message;
public:
    const char* what () const noexcept override {
        return m_message.c_str();
    }

    incompatible_type_error () : m_message("types are not compatible") {}
};

} /* namespace mlang */