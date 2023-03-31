#ifndef INDICATORS_DISPLAY_WIDTH_HPP
#define INDICATORS_DISPLAY_WIDTH_HPP

#include <string>
#include <wchar.h>

namespace unicode {

static inline int DisplayWidth(const std::string& input) {
    return input.length();
}

static inline int DisplayWidth(const std::wstring& input) {
    return input.length();
}

} // !namespace unicode

#endif // !INDICATORS_DISPLAY_WIDTH_HPP
