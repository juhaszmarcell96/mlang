#pragma once
#pragma once

#include <string>

namespace mlang {
namespace script {

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
    hashtag,               /* # */
    plus_plus,             /* ++ */
    dash_dash,             /* -- */
    plus_equal,            /* += */
    dash_equal,            /* -= */
    asterisk_equal,        /* *= */
    slash_equal,           /* /= */
    percent_equal,         /* %= */
    double_less_than,      /* << */
    double_greater_than,   /* >> */
    double_less_eq,        /* <<= */
    double_greater_eq,     /* >>= */
    ampersand_equal,       /* &= */
    caret_equal,           /* ^= */
    pipe_equal,            /* |= */
    less_equal,            /* <= */
    greater_equal,         /* >= */
    double_equal,          /* == */
    exclamation_equal,     /* != */
    double_ampersand,      /* && */
    double_pipe,           /* || */
    /* keywords */
    kw_none,               /* none */
    kw_true,               /* true */
    kw_false,              /* false */
    kw_if,                 /* if */
    kw_elif,               /* elif */
    kw_else,               /* else */
    kw_while,              /* while */
    kw_for,                /* for */
    kw_new,                /* new */
    kw_break,              /* break */
    kw_continue,           /* continue */
    kw_switch,             /* switch */
    kw_case,               /* case */
    kw_default,            /* default */
    kw_function,           /* function */
    kw_var,                /* var */
    kw_return,             /* return */
    kw_exit,               /* exit */
    kw_print               /* print */
};

struct Token {
    std::size_t line { 0 };
    std::size_t pos { 0 };
    token_types type;
    std::string value_str;         /* for strings and identifiers */
    double value_num { 0.0 };      /* for numbers */
    bool value_bool { false };     /* for booleans */

    Token (token_types token_type, const std::string& value, std::size_t line_num, std::size_t position) : type(token_type), value_str(value), line(line_num), pos(position)  {}
    Token (token_types token_type, const char* value, std::size_t line_num, std::size_t position) : type(token_type), value_str(value), line(line_num), pos(position)  {}
    Token (token_types token_type, double value, std::size_t line_num, std::size_t position) : type(token_type), value_num(value), line(line_num), pos(position)  {}
    Token (token_types token_type, int value, std::size_t line_num, std::size_t position) : type(token_type), value_num(value), line(line_num), pos(position)  {}
    Token (token_types token_type, bool value, std::size_t line_num, std::size_t position) : type(token_type), value_bool(value), line(line_num), pos(position)  {}
    Token (token_types token_type, std::size_t line_num, std::size_t position) : type(token_type), line(line_num), pos(position) {}

    std::size_t get_length () const;
    std::string get_for_print() const;
};

} /* namespace script */
} /* namespace mlang */