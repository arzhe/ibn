#include <stdlib.h>
#include <sys/syscall.h> // for ::syscall
#include <unistd.h> // for ::getpid

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

} // !namespace CurrentThread

} // !namespace ibn
