#ifndef IBN_LOG_PROCESSINFO_H
#define IBN_LOG_PROCESSINFO_H

#include <vector>
#include <sys/types.h>

#include "base/common/string_view.h"
#include "base/common/types.h"
#include "log/timestamp.h"

namespace ibn {

namespace ProcessInfo {

pid_t Pid();
std::string PidString();
uid_t Uid();
std::string Username();
uid_t Euid();
Timestamp StartTime();
int ClockTicksPerSecond();
int PageSize();
bool IsDebugBuild();

std::string Hostname();
std::string Procname();
StringView Procname(const std::string& stat);

// read /proc/self/status
std::string ProcStatus();

// read /proc/self/stat
std::string ProcStat();

// read /proc/self/task/tid/stat
std::string ThreadStat();

// readlink /proc/self/exe
std::string ExePath();

int OpenedFiles();
int MaxOpenFiles();

struct CpuTime {
    double user_seconds;
    double system_seconds;

    CpuTime() : user_seconds(0.0), system_seconds(0.0) {}

    double Total() const { return user_seconds + system_seconds; }
};

CpuTime GetCpuTime();

int NumThreads();
std::vector<pid_t> Threads();

} // !namespace ProcessInfo

} // !namespace ibn

#endif // !IBN_LOG_PROCESSINFO_H
