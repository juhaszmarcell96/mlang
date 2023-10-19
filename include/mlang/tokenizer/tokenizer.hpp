#pragma once

#include <string>
#include <vector>

#include "mlang/tokenizer/code.hpp"

namespace mlang {
namespace tokenizer {

enum class token_types {
    none,
    identifier,            /* variable names, etc */
    integer,               /* int */
    floating,              /* float */
    string,                /* string literal */
    comma,                 /* , */
    question_mark,         /* ? */
    colon,                 /* : */
    semicolon,             /* ; */
    round_bracket_open,    /* ( */
    round_bracket_close,   /* ) */
    square_bracket_open,   /* [ */
    square_bracket_close,  /* ] */
    curly_bracket_open,    /* { */
    curly_bracket_close,   /* } */
    asterisk,              /* * */
    slash,                 /* / */
    percent,               /* % */
    plus,                  /* + */
    dash,                  /* - */
    equal_sign,            /* = */
    exclamation_mark,      /* ! */
    tilde,                 /* ~ */
    less,                  /* < */
    greater,               /* > */
    ampersand,             /* & */
    dollar,                /* $ */
    caret,                 /* ^ */
    pipe,                  /* | */
    dot,                   /* . */
    at,                    /* @ */
    hashtag,               /* # */
};

class Token {
private:
    token_types m_type;
    std::string m_value;
    std::size_t m_line { 0 };
    std::size_t m_pos { 0 };
public:
    Token ();
    void set_type (token_types type);
    void append (char ch);
    void clear();

    const std::string& get_value () const;
    const token_types get_type () const;

    void set_line (std::size_t line);
    void set_pos (std::size_t pos);

    std::size_t get_line () const;
    std::size_t get_pos () const;
};

class Tokenizer {
private:
    Code m_code;
    std::vector<Token> m_tokens;

    enum class tokenizer_states {
        none,
        parsing_identifier,
        parsing_string,
        parsing_number
    };

    tokenizer_states m_current_state { tokenizer_states::none };
    char m_current_char { 0 };
    Token m_current_token {};
    bool m_escape { false };
    bool m_has_dot { false }; /* indicates whether a number already has a dot -> float */

    void start_new_token ();
    void start_new_token (token_types type);
    void finalize_token ();
    void finalize_token (token_types type);
    void set_token_type (token_types type);
    void append_curr ();
    void append_char (char c);
    void set_state (tokenizer_states state);

    void process_digit ();
    void process_whitespace ();
    void process_backslash ();
    void process_double_quotes ();
    void process_letter ();
    void process_apostrophe ();
    void process_dot ();
    void process_standalone_sign (token_types type);
public:
    Tokenizer () = delete;
    Tokenizer (const std::string& script);

    const std::vector<Token>& get_tokens () const;
    void clear ();

    void tokenize();
};

} /* namespace tokenizer */
} /* namespace mlang */