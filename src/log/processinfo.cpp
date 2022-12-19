#include <algorithm>

#include <assert.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>

#include "log/processinfo.h"
#include "log/file_util.h"
#include "base/common/current_thread.h"

namespace ibn {

namespace detail {

__thread int t_num_opened_files = 0;
int FdDirFilter(const struct dirent* d) {
    if(::isdigit(d->d_name[0])) {
        ++t_num_opened_files;
    }
    return 0;
}

__thread std::vector<pid_t>* t_pids = NULL;
int TaskDirFilter(const struct dirent* d) {
    if(::isdigit(d->d_name[0])) {
        t_pids->push_back(atoi(d->d_name));
    }
    return 0;
}

int ScanDir(const char* dirpath, int (*filter)(const struct dirent*)) {
    struct dirent** namelist = NULL;
    int result = ::scandir(dirpath, &namelist, filter, alphasort);
    assert(namelist == NULL);
    return result;
}

Timestamp g_start_time = Timestamp::Now();
int g_clock_ticks = static_cast<int>(::sysconf(_SC_CLK_TCK));
int g_page_size = static_cast<int>(::sysconf(_SC_PAGE_SIZE));

} // !namespace detail

using namespace detail;

pid_t ProcessInfo::Pid() {
    return ::getpid();
}

std::string ProcessInfo::PidString() {
    char buf[32];
    snprintf(buf, sizeof buf, "%d", Pid());
    return buf;
}

uid_t ProcessInfo::Uid() {
    return ::getuid();
}

std::string ProcessInfo::Username() {
    struct passwd pwd;
    struct passwd* result = NULL;
    char buf[8192];
    const char* name = "unknownuser";

    getpwuid_r(Uid(), &pwd, buf, sizeof buf, &result);
    if(result) {
        name = pwd.pw_name;
    }
    return name;
}

uid_t ProcessInfo::Euid() {
    return ::geteuid();
}

Timestamp ProcessInfo::StartTime() {
    return g_start_time;
}

int ProcessInfo::ClockTicksPerSecond() {
    return g_clock_ticks;
}

int ProcessInfo::PageSize() {
    return g_page_size;
}

bool ProcessInfo::IsDebugBuild() {
#ifndef NDEBUG
    return false;
#else
    return true;
#endif
}

std::string ProcessInfo::Hostname() {
    char buf[256];
    if(::gethostname(buf, sizeof buf) == 0) {
        buf[sizeof(buf) - 1] = '\0';
        return buf;
    }
    else {
        return "unknownhost";
    }
}

std::string ProcessInfo::Procname() {
    return Procname(ProcStat()).as_string();
}

StringView ProcessInfo::Procname(const std::string& stat) {
    StringView name;
    size_t lp = stat.find('(');
    size_t rp = stat.rfind(')');
    if(lp != std::string::npos && rp != std::string::npos && lp < rp) {
        name.set(stat.data() + lp + 1, static_cast<int>(rp - lp - 1));
    }
    return name;
}

std::string ProcessInfo::ProcStatus() {
    std::string result;
    FileUtil::ReadFile("/proc/self/status", 65536, &result);
    return result;
}

std::string  ProcessInfo::ProcStat() {
    std::string result;
    FileUtil::ReadFile("/proc/self/stat", 65536, &result);
    return result;
}

std::string ProcessInfo::ThreadStat() {
    char buf[64];
    snprintf(buf, sizeof buf, "/proc/self/task/%d/stat", CurrentThread::Tid());
    std::string result;
    FileUtil::ReadFile(buf, 65536, &result);
    return result;
}

std::string ProcessInfo::ExePath() {
    std::string result;
    char buf[1024];
    ssize_t n = ::readlink("/proc/self/exe", buf, sizeof buf);
    if(n > 0) {
        result.assign(buf, n);
    }
    return result;
}

int ProcessInfo::OpenedFiles() {
    t_num_opened_files = 0;
    ScanDir("/proc/self/fd", FdDirFilter);
    return t_num_opened_files;
}

int ProcessInfo::MaxOpenFiles() {
    struct rlimit rl;
    if(::getrlimit(RLIMIT_NOFILE, &rl)) {
        return OpenedFiles();
    }
    else {
        return static_cast<int>(rl.rlim_cur);
    }
}

ProcessInfo::CpuTime ProcessInfo::GetCpuTime() {
    ProcessInfo::CpuTime t;
    struct tms tms;
    if(::times(&tms) >= 0) {
        const double hz = static_cast<double>(ClockTicksPerSecond());
        t.user_seconds = static_cast<double>(tms.tms_utime) / hz;
        t.system_seconds = static_cast<double>(tms.tms_stime) / hz;
    }
    return t;
}

int ProcessInfo::NumThreads() {
    int result = 0;
    std::string status = ProcStatus();
    size_t pos = status.find("Threads:");
    if(pos != std::string::npos) {
        result = ::atoi(status.c_str() + pos + 8);
    }
    return result;
}

std::vector<pid_t> ProcessInfo::Threads() {
    std::vector<pid_t> result;
    t_pids = &result;
    ScanDir("/proc/self/task", TaskDirFilter);
    t_pids = NULL;
    std::sort(result.begin(), result.end());
    return result;
}

} // !namespace ibn

