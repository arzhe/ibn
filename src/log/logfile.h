#ifndef IBN_LOG_LOGFILE_H
#define IBN_LOG_LOGFILE_H

#include <memory>

#include "base/mutex/mutex.h"
#include "base/common/types.h"

namespace ibn {

namespace FileUtil {

class AppendFile;

} // !namespace FileUtil

class LogFile : noncopyable {
public:
    LogFile(const std::string& basename,
            /*
             *  32-bits > long int || 64-bits > long long int
             *  Defined in <unistd.h>.
             *  Used to indicate the offset of a file.
             */ 
            off_t roll_size,
            bool thread_safe = true,
            int flush_interval = 3,
            int check_every_n = 1024);

    ~LogFile();

    void Append(const char* logline, int len);
    void Flush();
    bool RollFile();

private:
    void AppendUnlocked(const char* logline, int len);

    static std::string GetLogFileName(const std::string& basename, time_t* now);

    const std::string basename_;
    const off_t roll_size_;
    const int flush_interval_;
    const int check_every_n_;

    int count_;

    std::unique_ptr<MutexLock> mutex_;
    time_t start_of_period_;
    time_t last_roll_;
    time_t last_flush_;
    std::unique_ptr<FileUtil::AppendFile> file_;

    const static int kRollPerSeconds_ = 60 * 60 * 24;
};

} // !namespace ibn

#endif // !IBN_LOG_LOGFILE_H
