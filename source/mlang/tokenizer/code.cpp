#include "mlang/tokenizer/code.hpp"

namespace mlang {

Code::Code (const std::string& code) : m_code(code) {}

char Code::get_current_char () const { return m_code[m_index]; }

bool Code::get_next_char (char& ch) const {
    if (m_index + 1 >= m_code.length()) return false;
    return m_code[m_index + 1];
}

bool Code::get_nth_next_char (char& ch, std::size_t n) const {
    if (m_index + n >= m_code.length()) return false;
    return m_code[m_index + n];
}

bool Code::step () {
    ++m_index;
    if (m_index >= m_code.length()) return false;
    if (m_code[m_index] == '\n') {
        ++m_line_num;
        m_column = 0;
    }
    else {
        ++m_column;
    }
    return true;
}

std::size_t Code::get_index () const { return m_index; }

std::size_t Code::get_line_num () const { return m_line_num; }

std::size_t Code::get_column () const { return m_column; }

void Code::clear () { m_code.clear(); }

} /* namespace mlang */