#ifndef IBN_BASE_COMMON_EXCEPTION_H
#define IBN_BASE_COMMON_EXCEPTION_H

#include <exception>
#include "base/common/types.h"

namespace ibn {

// C-style
class Exception : public std::exception {
public:
    Exception(std::string what);
    ~Exception() noexcept override = default;

    // default copy-ctor and operator= are okay.
    
    const char* what() const noexcept override {
        return message_.c_str();
    }

    const char* stack_trace() const noexcept {
        return stack_.c_str();
    }

private:
    std::string message_;
    std::string stack_;
};


} // !namespace ibn

#endif // !IBN_BASE_COMMON_EXCEPTION_H
