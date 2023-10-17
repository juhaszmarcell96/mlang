#include "mlang/script/script.hpp"
#include "mlang/tokenizer/tokenizer.hpp"
#include "mlang/exception.hpp"
#include "mlang/object/object.hpp"
#include "mlang/parser/parser.hpp"

#define DEBUG_SCRIPT 0

#if DEBUG_SCRIPT == 1
#include <iostream>
#endif

namespace mlang {
namespace script {

void Script::debug(const std::string& debug_message) {
    #if DEBUG_SCRIPT == 1
        std::cout << debug_message << std::endl;
    #endif
}

Script::Script (const std::string& script) {
    tokenizer::Tokenizer tokenizer { script };
    tokenizer.tokenize();

    const std::vector<tokenizer::Token>& tokens = tokenizer.get_tokens();
    std::size_t tokens_size = tokens.size();
    for (std::size_t index = 0; index < tokens_size; ++index) {
        const tokenizer::Token& token = tokens[index];
        switch (token.get_type()) {
            case tokenizer::token_types::none: {
                throw SyntaxError{"token cannot be of type none", token.get_line(), token.get_pos()};
            }
            case tokenizer::token_types::identifier: {
                if (token.get_value().compare("none") == 0) {
                    debug("token with type 'none'");
                    m_tokens.push_back(Token{token_types::kw_none, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("true") == 0) {
                    debug("token with type 'true'");
                    m_tokens.push_back(Token{token_types::kw_true, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("false") == 0) {
                    debug("token with type 'false'");
                    m_tokens.push_back(Token{token_types::kw_false, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("if") == 0) {
                    debug("token with type 'if'");
                    m_tokens.push_back(Token{token_types::kw_if, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("elif") == 0) {
                    debug("token with type 'elif'");
                    m_tokens.push_back(Token{token_types::kw_elif, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("else") == 0) {
                    debug("token with type 'else'");
                    m_tokens.push_back(Token{token_types::kw_else, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("new") == 0) {
                    debug("token with type 'new'");
                    m_tokens.push_back(Token{token_types::kw_new, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("for") == 0) {
                    debug("token with type 'for'");
                    m_tokens.push_back(Token{token_types::kw_for, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("while") == 0) {
                    debug("token with type 'while'");
                    m_tokens.push_back(Token{token_types::kw_while, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("break") == 0) {
                    debug("token with type 'break'");
                    m_tokens.push_back(Token{token_types::kw_break, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("continue") == 0) {
                    debug("token with type 'continue'");
                    m_tokens.push_back(Token{token_types::kw_continue, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("switch") == 0) {
                    debug("token with type 'switch'");
                    m_tokens.push_back(Token{token_types::kw_switch, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("case") == 0) {
                    debug("token with type 'case'");
                    m_tokens.push_back(Token{token_types::kw_case, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("default") == 0) {
                    debug("token with type 'default'");
                    m_tokens.push_back(Token{token_types::kw_default, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("function") == 0) {
                    debug("token with type 'function'");
                    m_tokens.push_back(Token{token_types::kw_function, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("var") == 0) {
                    debug("token with type 'variable'");
                    m_tokens.push_back(Token{token_types::kw_var, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("return") == 0) {
                    debug("token with type 'return'");
                    m_tokens.push_back(Token{token_types::kw_return, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("exit") == 0) {
                    debug("token with type 'exit'");
                    m_tokens.push_back(Token{token_types::kw_exit, token.get_line(), token.get_pos()});
                }
                else if (token.get_value().compare("print") == 0) {
                    debug("token with type 'print'");
                    m_tokens.push_back(Token{token_types::kw_print, token.get_line(), token.get_pos()});
                }
                else {
                    debug("token with type 'identifier' and value " + token.get_value());
                    m_tokens.push_back(Token{token_types::identifier, token.get_value(), token.get_line(), token.get_pos()});
                }
                break;
            }
            case tokenizer::token_types::number: {
                try {
                    double value = std::stod(token.get_value());
                    debug("token with type 'number' and value " + std::to_string(value));
                    m_tokens.push_back(Token{token_types::number, value, token.get_line(), token.get_pos()});
                }
                catch (const std::invalid_argument&) {
                    throw SyntaxError{"number token could not be converted, invalid format", token.get_line(), token.get_pos()};
                }
                catch (const std::out_of_range&) {
                    throw SyntaxError{"number token could not be converted, out of range", token.get_line(), token.get_pos()};
                }
                break;
            }
            case tokenizer::token_types::string: {
                debug("token with type 'string' and value " +  token.get_value());
                m_tokens.push_back(Token{token_types::string, token.get_value(), token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::comma: {
                debug("token with type 'comma'");
                m_tokens.push_back(Token{token_types::comma, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::question_mark: {
                debug("token with type 'question_mark'");
                m_tokens.push_back(Token{token_types::question_mark, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::colon: {
                debug("token with type 'colon'");
                m_tokens.push_back(Token{token_types::colon, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::semicolon: {
                debug("token with type 'semicolon'");
                m_tokens.push_back(Token{token_types::semicolon, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::round_bracket_open: {
                debug("token with type 'round_bracket_open'");
                m_tokens.push_back(Token{token_types::round_bracket_open, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::round_bracket_close: {
                debug("token with type 'round_bracket_close'");
                m_tokens.push_back(Token{token_types::round_bracket_close, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::square_bracket_open: {
                debug("token with type 'square_bracket_open'");
                m_tokens.push_back(Token{token_types::square_bracket_open, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::square_bracket_close: {
                debug("token with type 'square_bracket_close'");
                m_tokens.push_back(Token{token_types::square_bracket_close, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::curly_bracket_open: {
                debug("token with type 'curly_bracket_open'");
                m_tokens.push_back(Token{token_types::curly_bracket_open, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::curly_bracket_close: {
                debug("token with type 'curly_bracket_close'");
                m_tokens.push_back(Token{token_types::curly_bracket_close, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::asterisk: {
                /* it could be a signle asterisk, or it could be '*=' */
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'asterisk_equal'");
                        m_tokens.push_back(Token{token_types::asterisk_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'asterisk'");
                m_tokens.push_back(Token{token_types::asterisk, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::slash: {
                /* it could be a signle slash, or it could be slash_equal */
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'slash_equal'");
                        m_tokens.push_back(Token{token_types::slash_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'slash'");
                m_tokens.push_back(Token{token_types::slash, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::percent: {
                /* it could be a signle percent, or it could be percent_equal */
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'percent_equal'");
                        m_tokens.push_back(Token{token_types::percent_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'percent'");
                m_tokens.push_back(Token{token_types::percent, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::plus: {
                /* it could be a signle plus, or it could be plus_equal, or plus_plus */
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'plus_equal'");
                        m_tokens.push_back(Token{token_types::plus_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::plus) {
                        debug("token with type 'plus_plus'");
                        m_tokens.push_back(Token{token_types::plus_plus, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'plus'");
                m_tokens.push_back(Token{token_types::plus, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::dash: {
                /* it could be a signle dash, or it could be dash_equal, or dash_dash */
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'dash_equal'");
                        m_tokens.push_back(Token{token_types::dash_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::dash) {
                        debug("token with type 'dash_dash'");
                        m_tokens.push_back(Token{token_types::dash_dash, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'dash'");
                m_tokens.push_back(Token{token_types::dash, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::equal_sign: {
                /* it could be a signle dash, or it could be double_equal */
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'double_equal'");
                        m_tokens.push_back(Token{token_types::double_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'equal_sign'");
                m_tokens.push_back(Token{token_types::equal_sign, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::exclamation_mark: {
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'exclamation_equal'");
                        m_tokens.push_back(Token{token_types::exclamation_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'exclamation_mark'");
                m_tokens.push_back(Token{token_types::exclamation_mark, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::tilde: {
                debug("token with type 'tilde'");
                m_tokens.push_back(Token{token_types::tilde, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::less: {
                /* it could be a '<', '<<', '<=', '<<=' */
                if ((index + 1) < tokens_size) {
                    if ((index + 2) < tokens_size) {
                        if (tokens[index + 1].get_type() == tokenizer::token_types::less) {
                            if (tokens[index + 2].get_type() == tokenizer::token_types::equal_sign) {
                                debug("token with type 'double_less_eq'");
                                m_tokens.push_back(Token{token_types::double_less_eq, token.get_line(), token.get_pos()});
                                index += 2;
                                break;
                            }
                        }
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'less_equal'");
                        m_tokens.push_back(Token{token_types::less_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::less) {
                        debug("token with type 'double_less_than'");
                        m_tokens.push_back(Token{token_types::double_less_than, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'less'");
                m_tokens.push_back(Token{token_types::less, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::greater: {
                /* it could be a '>', '>>', '>=', '>>=' */
                if ((index + 1) < tokens_size) {
                    if ((index + 2) < tokens_size) {
                        if (tokens[index + 1].get_type() == tokenizer::token_types::greater) {
                            if (tokens[index + 2].get_type() == tokenizer::token_types::equal_sign) {
                                debug("token with type 'double_greater_eq'");
                                m_tokens.push_back(Token{token_types::double_greater_eq, token.get_line(), token.get_pos()});
                                index += 2;
                                break;
                            }
                        }
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'greater_equal'");
                        m_tokens.push_back(Token{token_types::greater_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::greater) {
                        debug("token with type 'double_greater_than'");
                        m_tokens.push_back(Token{token_types::double_greater_than, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'greater'");
                m_tokens.push_back(Token{token_types::greater, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::ampersand: {
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'ampersand_equal'");
                        m_tokens.push_back(Token{token_types::ampersand_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::ampersand) {
                        debug("token with type 'double_ampersand'");
                        m_tokens.push_back(Token{token_types::double_ampersand, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'ampersand'");
                m_tokens.push_back(Token{token_types::ampersand, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::dollar: {
                debug("token with type 'dollar'");
                m_tokens.push_back(Token{token_types::dollar, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::caret: {
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'caret_equal'");
                        m_tokens.push_back(Token{token_types::caret_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'caret'");
                m_tokens.push_back(Token{token_types::caret, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::pipe: {
                if ((index + 1) < tokens_size) {
                    if (tokens[index + 1].get_type() == tokenizer::token_types::equal_sign) {
                        debug("token with type 'pipe_equal'");
                        m_tokens.push_back(Token{token_types::pipe_equal, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                    if (tokens[index + 1].get_type() == tokenizer::token_types::pipe) {
                        debug("token with type 'double_pipe'");
                        m_tokens.push_back(Token{token_types::double_pipe, token.get_line(), token.get_pos()});
                        ++index;
                        break;
                    }
                }
                debug("token with type 'pipe'");
                m_tokens.push_back(Token{token_types::pipe, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::dot: {
                debug("token with type 'dot'");
                m_tokens.push_back(Token{token_types::dot, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::at: {
                debug("token with type 'at'");
                m_tokens.push_back(Token{token_types::at, token.get_line(), token.get_pos()});
                break;
            }
            case tokenizer::token_types::hashtag: {
                debug("token with type 'hashtag'");
                m_tokens.push_back(Token{token_types::hashtag, token.get_line(), token.get_pos()});
                break;
            }
            default: {
                throw SyntaxError{"unknown token type", token.get_line(), token.get_pos()};
            }
        }
    }
}

    
const std::vector<Token>& Script::get_tokens () const { return m_tokens; }

int Script::execute (script::EnvStack& env) {
    parser::Parser parser {};
    ast::node_ptr root {};
    try {
        root = parser.parse( m_tokens );
    }
    catch (const SyntaxError& e) {
        std::cout << "ERROR : syntax error occurred" << std::endl;
        std::cout << e.what() << std::endl;
        return 1;
    }
    //root->print();
    try {
        root->execute(env);
    }
    catch (const RuntimeError& e) {
        std::cout << "ERROR : runtime error occurred" << std::endl;
        std::cout << e.what() << std::endl;
        return 2;
    }
    return 0;
}

} /* namespace script */
} /* namespace mlang */