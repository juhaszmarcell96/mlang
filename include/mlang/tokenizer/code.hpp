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
    Code (const std::string& code);

    char get_current_char () const;
    bool get_next_char (char& ch) const;
    bool get_nth_next_char (char& ch, std::size_t n) const;
    bool step ();
    std::size_t get_index () const;
    std::size_t get_line_num () const;
    std::size_t get_column () const;

    void clear ();
};

} /* namespace mlang */