#include <stdlib.h>
#include <sys/syscall.h> // for ::syscall
#include <unistd.h> // for ::getpid
#include <cxxabi.h>
#include <execinfo.h>

#include "base/common/current_thread.h"

namespace ibn {

namespace CurrentThread {

__thread int t_cached_tid = 0;
__thread char t_tid_string[32];
__thread int t_tid_string_length = 6;
__thread const char* t_thread_name = "unknown";
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

void CacheTid() {
    if(t_cached_tid == 0) {
        t_cached_tid = static_cast<pid_t>(::syscall(SYS_gettid));
        t_tid_string_length = snprintf(t_tid_string, sizeof t_tid_string, "%5d", t_cached_tid);
    }
}

bool IsMainThread() {
    return Tid() == ::getpid();
}

std::string StackTrace(bool demangle) {
    std::string stack;
    const int max_frames = 200;
    void* frame[max_frames];
    int nptrs = ::backtrace(frame, max_frames);
    char** strings = ::backtrace_symbols(frame, nptrs);
    if(strings) {
        size_t len = 256;
        char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
        for(int i = 1; i < nptrs; ++i) { // skipping the 0-th, which is this function
            if(demangle) {
                char* left_par = nullptr;
                char* plus = nullptr;
                for(char* p = strings[i]; *p; ++p) {
                    if(*p == '(') {
                        left_par = p;
                    }
                    else if(*p == '+') {
                        plus = p;
                    }
                }

                if(left_par && plus) {
                    *plus = '\0';
                    int status = 0;
                    char* ret = abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
                    *plus = '+';
                    if(status == 0) {
                        demangled = ret; // ret should be realloc()
                        stack.append(strings[i], left_par + 1);
                        stack.append(demangled);
                        stack.append(plus);
                        stack.push_back('\n');
                        continue;
                    }
                }
            }
            // fallback to mangled names
            stack.append(strings[i]);
            stack.push_back('\n');
        }
        free(demangled);
        free(strings);
    }
    return stack;
}

} // !namespace CurrentThread

} // !namespace ibn
