#ifndef IBN_BASE_COMMON_CURRENT_THREAD_H
#define IBN_BASE_COMMON_CURRENT_THREAD_H

#include "base/common/types.h"

namespace ibn {

namespace CurrentThread {

extern __thread int t_cached_tid;
extern __thread char t_tid_string[32];
extern __thread int t_tid_string_length;
extern __thread const char* t_thread_name;

void CacheTid();

inline int Tid() {
    if(__builtin_expect(t_cached_tid == 0, 0)) {
        CacheTid();
    }
    return t_cached_tid;
}

inline const char* TidString() {
    return t_tid_string;
}

inline int TidStringLength() {
    return t_tid_string_length;
}

inline const char* Name() {
    return t_thread_name;
}

bool IsMainThread();

std::string StackTrace(bool demangle);

} // !namespace CurrentThread

} // !namespace ibn

#endif // !IBN_BASE_COMMON_CURRENT_THREAD_H
