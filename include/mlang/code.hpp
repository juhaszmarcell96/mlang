#pragma once

#include <string>

namespace mlang {

class Code {
private:
    std::string m_code;
    std::size_t m_index { (std::size_t)-1 };
    std::size_t m_line_num { 1 };
    std::size_t m_column { 0 };
public:
    Code () = delete;
    Code (const std::string& code) : m_code(code) {}

    char get_current_char () const { return m_code[m_index]; }
    bool get_next_char (char& ch) const {
        if (m_index + 1 >= m_code.length()) return false;
        return m_code[m_index + 1];
    }
    bool get_nth_next_char (char& ch, std::size_t n) const {
        if (m_index + n >= m_code.length()) return false;
        return m_code[m_index + n];
    }
    bool step () {
        ++m_index;
        if (m_index >= m_code.length()) return false;
        if (m_code[m_index] == '\n') {
            ++m_line_num;
            m_column = 1;
        }
        else {
            ++m_column;
        }
        return true;
    }
    std::size_t get_index () const { return m_index; }
    std::size_t get_line_num () const { return m_line_num; }
    std::size_t get_column () const { return m_column; }

    void clear () { m_code.clear(); }
};

} /* namespace mlang */