#ifndef INDICATORS_CURSOR_CONTROL_HPP
#define INDICATORS_CURSOR_CONTROL_HPP

#include <cstdio>

namespace indicators {

static inline void ShowConsoleCursor(bool const show) {
    std::fputs(show ? "\033[?25h" : "\033[?25l", stdout);
}

static inline void EraseLine() {
    std::fputs("\r\033[K", stdout);
}

} // !namespace indicators

#endif // !INDICATORS_CURSOR_CONTROL_HPP
