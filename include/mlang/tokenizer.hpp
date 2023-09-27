#pragma once

#include <string>
#include <vector>

#include "code.hpp"
#include "exception.hpp"

namespace lang {
namespace tokenizer {

enum class token_types {
    none,
    identifier,            /* variable names, etc */
    number,                /* number */
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
    hashtag                /* # */
};

class Token {
private:
    token_types m_type;
    std::string m_value;
    std::size_t m_line { 0 };
    std::size_t m_pos { 0 };
public:
    Token () : m_type(token_types::none) {}
    void set_type (token_types type) { m_type = type; }
    void append (char ch) { m_value.push_back(ch); }
    void clear() {
        m_type = token_types::none;
        m_value.clear();
        m_line = 0;
        m_pos = 0;
    }
    const std::string& get_value () const { return m_value; }
    const token_types get_type () const { return m_type; }

    void set_line (std::size_t line) { m_line = line; }
    void set_pos (std::size_t pos) { m_pos = pos; }

    std::size_t get_line () const { return m_line; }
    std::size_t get_pos () const  { return m_pos; }
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

    void process_number () {
        if (m_current_state == tokenizer_states::none) {
            /* no tokens were in progress -> start a new one -> number */
            m_current_token.clear();
            m_current_token.append(m_current_char);
            m_current_state = tokenizer_states::parsing_number;
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else {
            /* tokenizer_states::parsing_identifier */
            /* tokenizer_states::parsing_string */
            /* tokenizer_states::parsing_number */
            /* just append the number */
            m_current_token.append(m_current_char);
        }
    }

    void process_whitespace () {
        if (m_current_state == tokenizer_states::none) {
            /* no tokens were in progress -> just move on */
        }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* parsing an identifier -> finalize this token */
            m_current_token.set_type(token_types::identifier);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_state = tokenizer_states::none;
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* parsing a string -> append, space can be part of a string */
            m_current_token.append(m_current_char);
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* parsing a number -> finalize this token */
            m_current_token.set_type(token_types::number);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_state = tokenizer_states::none;
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
    }

    void process_backslash () {
        if (m_current_state == tokenizer_states::none) {
            /* backslash without a token? -> syntax error */
            throw syntax_error{"character '\\' at invalid position", m_code.get_line_num(), m_code.get_column()};
        }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* backslash in identiefer? -> syntax error */
            throw syntax_error{"character '\\' at invalid position", m_code.get_line_num(), m_code.get_column()};
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* parsing a string -> backslash in string can be tricky */
            if (!m_escape) m_escape = true;
            else {
                m_current_token.append(m_current_char);
                m_escape = false;
            }
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* parsing a number -> no backslash in number -> syntax error */
            throw syntax_error{"character '\\' at invalid position", m_code.get_line_num(), m_code.get_column()};
        }
    }

    void process_double_quotes () {
        if (m_current_state == tokenizer_states::none) {
            /* start a string */
            m_current_state = tokenizer_states::parsing_string;
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* end the identifier, start a string */
            m_current_token.set_type(token_types::identifier);
            m_tokens.push_back(m_current_token);
            m_current_state = tokenizer_states::parsing_string;
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* parsing a string -> if escaped, then just add to string, if not, then end string */
            if (m_escape) {
                m_current_token.append(m_current_char);
                m_escape = false;
            }
            else {
                m_current_token.set_type(token_types::string);
                m_tokens.push_back(m_current_token);
                m_current_state = tokenizer_states::none;
                m_current_token.clear();
                m_current_token.set_line(m_code.get_line_num());
                m_current_token.set_pos(m_code.get_column());
            }
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* end the number, start a string */
            m_current_token.set_type(token_types::number);
            m_tokens.push_back(m_current_token);
            m_current_state = tokenizer_states::parsing_string;
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
    }

    void process_letter () {
        if (m_current_state == tokenizer_states::none) {
            /* start an identifier */
            m_current_state = tokenizer_states::parsing_identifier;
            m_current_token.clear();
            m_current_token.append(m_current_char);
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* append to identifier */
            m_current_token.append(m_current_char);
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* parsing a string -> escape or not */
            if (m_escape) {
                if (m_current_char == 'n') m_current_token.append('\n');
                else if (m_current_char == 't') m_current_token.append('\t');
                else if (m_current_char == 'r') m_current_token.append('\r');
                else throw syntax_error{"invalid escape sequence", m_code.get_line_num(), m_code.get_column()};
                m_escape = false;
            }
            else {
                m_current_token.append(m_current_char);
            }
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* end the number, start an identifier */
            m_current_token.set_type(token_types::number);
            m_tokens.push_back(m_current_token);
            m_current_state = tokenizer_states::parsing_identifier;
            m_current_token.clear();
            m_current_token.append(m_current_char);
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
    }

    void process_dot () {
        if (m_current_state == tokenizer_states::none) {
            /* start a number with 0.xxx */
            m_current_state = tokenizer_states::parsing_number;
            m_current_token.clear();
            m_current_token.append('0');
            m_current_token.append(m_current_char);
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* end identifier and start a new one -> . -> end and clear */
            m_current_token.set_type(token_types::identifier);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_token.append(m_current_char);
            m_current_token.set_type(token_types::dot);
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_state = tokenizer_states::none;
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* parsing a string -> escape or not */
            if (m_escape) {
                throw syntax_error{"invalid escape sequence", m_code.get_line_num(), m_code.get_column()};
            }
            else {
                m_current_token.append(m_current_char);
            }
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* append to number -> float */
            m_current_token.append(m_current_char);
        }
    }

    void process_standalone_sign (token_types type) {
        if (m_current_state == tokenizer_states::none) {
            /* equal sign */
            m_current_token.clear();
            m_current_token.append(m_current_char);
            m_current_token.set_type(type);
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* end identifier and start a new one -> end and clear */
            m_current_token.set_type(token_types::identifier);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
            m_current_token.append(m_current_char);
            m_current_token.set_type(type);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_state = tokenizer_states::none;
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* parsing a string -> escape or not */
            if (m_escape) {
                throw syntax_error{"invalid escape sequence", m_code.get_line_num(), m_code.get_column()};
            }
            else {
                m_current_token.append(m_current_char);
            }
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* end number and start a new one -> end and clear */
            m_current_token.set_type(token_types::number);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
            m_current_token.append(m_current_char);
            m_current_token.set_type(type);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
            m_current_token.set_line(m_code.get_line_num());
            m_current_token.set_pos(m_code.get_column());
            m_current_state = tokenizer_states::none;
        }
    }
public:
    Tokenizer () = delete;
    Tokenizer (const std::string& script) : m_code(script) {}

    const std::vector<Token>& get_tokens () const { return m_tokens; }
    void clear () { m_tokens.clear(); }

    void tokenize() {
        m_current_state = tokenizer_states::none;
        while (m_code.step()) {
            m_current_char = m_code.get_current_char();
            switch (m_current_char) {
                case '\t' :
                case '\n' :
                case '\r' :
                case ' ' : {
                    process_whitespace();
                    break;
                }
                case '0' :
                case '1' :
                case '2' :
                case '3' :
                case '4' :
                case '5' :
                case '6' :
                case '7' :
                case '8' :
                case '9' : {
                    process_number();
                    break;
                }
                case '\\': {
                    process_backslash();
                    break;
                }
                case '"': {
                    process_double_quotes();
                    break;
                }
                case 'a' :
                case 'b' :
                case 'c' :
                case 'd' :
                case 'e' :
                case 'f' :
                case 'g' :
                case 'h' :
                case 'i' :
                case 'j' :
                case 'k' :
                case 'l' :
                case 'm' :
                case 'n' :
                case 'o' :
                case 'p' :
                case 'q' :
                case 'r' :
                case 's' :
                case 't' :
                case 'u' :
                case 'v' :
                case 'w' :
                case 'x' :
                case 'y' :
                case 'z' :
                case '_' :
                case 'A' :
                case 'B' :
                case 'C' :
                case 'D' :
                case 'E' :
                case 'F' :
                case 'G' :
                case 'H' :
                case 'I' :
                case 'J' :
                case 'K' :
                case 'L' :
                case 'M' :
                case 'N' :
                case 'O' :
                case 'P' :
                case 'Q' :
                case 'R' :
                case 'S' :
                case 'T' :
                case 'U' :
                case 'V' :
                case 'W' :
                case 'X' :
                case 'Y' :
                case 'Z' : {
                    process_letter();
                    break;
                }
                case '.' : {
                    process_dot();
                    break;
                }
                case '=' : {
                    process_standalone_sign(token_types::equal_sign);
                    break;
                }
                case '(' : {
                    process_standalone_sign(token_types::round_bracket_open);
                    break;
                }
                case ')' : {
                    process_standalone_sign(token_types::round_bracket_close);
                    break;
                }
                case '{' : {
                    process_standalone_sign(token_types::curly_bracket_open);
                    break;
                }
                case '}' : {
                    process_standalone_sign(token_types::curly_bracket_close);
                    break;
                }
                case '[' : {
                    process_standalone_sign(token_types::square_bracket_open);
                    break;
                }
                case ']' : {
                    process_standalone_sign(token_types::square_bracket_close);
                    break;
                }
                case '+' : {
                    process_standalone_sign(token_types::plus);
                    break;
                }
                case '-' : {
                    process_standalone_sign(token_types::dash);
                    break;
                }
                case '<' : {
                    process_standalone_sign(token_types::less);
                    break;
                }
                case '>' : {
                    process_standalone_sign(token_types::greater);
                    break;
                }
                case '~' : {
                    process_standalone_sign(token_types::tilde);
                    break;
                }
                case '!' : {
                    process_standalone_sign(token_types::exclamation_mark);
                    break;
                }
                case '?' : {
                    process_standalone_sign(token_types::question_mark);
                    break;
                }
                case '@' : {
                    process_standalone_sign(token_types::at);
                    break;
                }
                case ':' : {
                    process_standalone_sign(token_types::colon);
                    break;
                }
                case ';' : {
                    process_standalone_sign(token_types::semicolon);
                    break;
                }
                case ',' : {
                    process_standalone_sign(token_types::comma);
                    break;
                }
                case '&' : {
                    process_standalone_sign(token_types::ampersand);
                    break;
                }
                case '|' : {
                    process_standalone_sign(token_types::pipe);
                    break;
                }
                case '*' : {
                    process_standalone_sign(token_types::asterisk);
                    break;
                }
                case '/' : {
                    process_standalone_sign(token_types::slash);
                    break;
                }
                case '%' : {
                    process_standalone_sign(token_types::percent);
                    break;
                }
                case '^' : {
                    process_standalone_sign(token_types::caret);
                    break;
                }
                case '$' : {
                    process_standalone_sign(token_types::dollar);
                    break;
                }
                case '#' : {
                    process_standalone_sign(token_types::hashtag);
                    break;
                }
                default: {
                    throw syntax_error{"unknown character ", m_code.get_line_num(), m_code.get_column()};
                }
            }
        }
        /* p[rocess the last token if in progress */
        if (m_current_state == tokenizer_states::none) { /* nothing to do */ }
        else if (m_current_state == tokenizer_states::parsing_identifier) {
            /* end identifier */
            m_current_token.set_type(token_types::identifier);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
        }
        else if (m_current_state == tokenizer_states::parsing_string) {
            /* string can only end in " -> error */
            throw syntax_error{"string not closed", m_code.get_line_num(), m_code.get_column()};
        }
        else if (m_current_state == tokenizer_states::parsing_number) {
            /* end number */
            m_current_token.set_type(token_types::number);
            m_tokens.push_back(m_current_token);
            m_current_token.clear();
        }

        /* clear the code string, we don't need it anymore */
        m_code.clear();
    }
};

} /* namespace tokenizer */
} /* namespace lang */