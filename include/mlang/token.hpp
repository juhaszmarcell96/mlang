#pragma once
#pragma once

#include <string>

#define DEBUG_SCRIPT 1

#if DEBUG_SCRIPT == 1
#include <iostream>
#endif

namespace lang {

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
    double_ampresend,      /* && */
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
    kw_break,              /* break */
    kw_continue,           /* continue */
    kw_switch,             /* switch */
    kw_case,               /* case */
    kw_default,            /* default */
    kw_function,           /* function */
    kw_number,             /* number */
    kw_string,             /* string */
    kw_vector,             /* vector */
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

    Token (token_types token_type, const std::string& value, std::size_t line_num, std::size_t position) : type(token_type), value_str(value), line(line_num), pos(position)  {}
    Token (token_types token_type, double value, std::size_t line_num, std::size_t position) : type(token_type), value_num(value), line(line_num), pos(position)  {}
    Token (token_types token_type, std::size_t line_num, std::size_t position) : type(token_type), line(line_num), pos(position) {}

    std::string get_for_print() const {
        switch (type) {
            case token_types::none : { return "none"; }
            case token_types::identifier : { return "identifier:" + value_str; }
            case token_types::number : { return "number:" + std::to_string(value_num); }
            case token_types::string : { return "string:" + value_str; }
            case token_types::comma : { return ","; }
            case token_types::question_mark : { return "?"; }
            case token_types::colon : { return ":"; }
            case token_types::semicolon : { return ";"; }
            case token_types::round_bracket_open : { return "("; }
            case token_types::round_bracket_close : { return ")"; }
            case token_types::square_bracket_open : { return "["; }
            case token_types::square_bracket_close : { return "]"; }
            case token_types::curly_bracket_open : { return "{"; }
            case token_types::curly_bracket_close : { return "}"; }
            case token_types::asterisk : { return "*"; }
            case token_types::slash : { return "/"; }
            case token_types::percent : { return "%"; }
            case token_types::plus : { return "+"; }
            case token_types::dash : { return "-"; }
            case token_types::equal_sign : { return "="; }
            case token_types::exclamation_mark : { return "!"; }
            case token_types::tilde : { return "~"; }
            case token_types::less : { return "<"; }
            case token_types::greater : { return ">"; }
            case token_types::ampersand : { return "&"; }
            case token_types::dollar : { return "$"; }
            case token_types::caret : { return "^"; }
            case token_types::pipe : { return "|"; }
            case token_types::dot : { return "."; }
            case token_types::at : { return "@"; }
            case token_types::hashtag : { return "#"; }
            case token_types::plus_plus : { return "++"; }
            case token_types::dash_dash : { return "--"; }
            case token_types::plus_equal : { return "+="; }
            case token_types::dash_equal : { return "-="; }
            case token_types::asterisk_equal : { return "*="; }
            case token_types::slash_equal : { return "/="; }
            case token_types::percent_equal : { return "%="; }
            case token_types::double_less_than : { return "<<"; }
            case token_types::double_greater_than : { return ">>"; }
            case token_types::double_less_eq : { return "<<="; }
            case token_types::double_greater_eq : { return ">>="; }
            case token_types::ampersand_equal : { return "&="; }
            case token_types::caret_equal : { return "^="; }
            case token_types::pipe_equal : { return "|="; }
            case token_types::less_equal : { return "<="; }
            case token_types::greater_equal : { return ">="; }
            case token_types::double_equal : { return "=="; }
            case token_types::exclamation_equal : { return "!="; }
            case token_types::double_ampresend : { return "&&"; }
            case token_types::double_pipe : { return "||"; }
            case token_types::kw_none : { return "none"; }
            case token_types::kw_true : { return "true"; }
            case token_types::kw_false : { return "false"; }
            case token_types::kw_if : { return "if"; }
            case token_types::kw_elif : { return "elif"; }
            case token_types::kw_else : { return "else"; }
            case token_types::kw_while : { return "while"; }
            case token_types::kw_for : { return "for"; }
            case token_types::kw_break : { return "break"; }
            case token_types::kw_continue : { return "continue"; }
            case token_types::kw_switch : { return "switch"; }
            case token_types::kw_case : { return "case"; }
            case token_types::kw_default : { return "default"; }
            case token_types::kw_function : { return "function"; }
            case token_types::kw_number : { return "number"; }
            case token_types::kw_string : { return "string"; }
            case token_types::kw_vector : { return "vector"; }
            case token_types::kw_return : { return "return"; }
            case token_types::kw_exit : { return "exit"; }
            case token_types::kw_print : { return "print"; }
            default: { return "ERROR"; }
        }
    }
};

} /* namespace lang */