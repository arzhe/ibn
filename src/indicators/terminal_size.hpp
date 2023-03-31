#ifndef INDICATORS_TERMINAL_SIZE_HPP
#define INDICATORS_TERMINAL_SIZE_HPP

#include <utility>     // std::pair
#include <sys/ioctl.h> // ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO


namespace indicators {

static inline std::pair<size_t, size_t> TerminalSize() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return { static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col) };
}

static inline size_t TerminalWidth() {
    return TerminalSize().second;
}

} // !namespace indicators


#endif // !INDICATORS_TERMINAL_SIZE_HPP
