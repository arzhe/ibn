#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "log/logfile.h"
#include "log/file_util.h"
#include "log/processinfo.h"

namespace ibn {

LogFile::LogFile(const std::string& basename,
                 off_t roll_size,
                 bool thread_safe,
                 int flush_interval,
                 int check_every_n)
    : basename_(basename),
      roll_size_(roll_size),
      flush_interval_(flush_interval),
      check_every_n_(check_every_n),
      count_(0),
      mutex_(thread_safe ? new MutexLock : NULL),
      start_of_period_(0),
      last_roll_(0),
      last_flush_(0) {
    assert(basename.find('/') == std::string::npos);
    RollFile();
}

LogFile::~LogFile() = default;

void LogFile::Append(const char* logline, int len) {
    if(mutex_) {
        MutexLockGuard lock(*mutex_);
        AppendUnlocked(logline, len);
    }
    else {
        AppendUnlocked(logline, len);
    }
}

void LogFile::Flush() {
    if(mutex_) {
        MutexLockGuard lock(*mutex_);
        file_->Flush();
    }
    else {
        file_->Flush();
    }
}

void LogFile::AppendUnlocked(const char* logline, int len) {
    file_->Append(logline, len);

    if(file_->WrittenBytes() > roll_size_) {
        RollFile();
    }
    else {
        ++count_;
        if(count_ >= check_every_n_) {
            count_ = 0;
            time_t now = ::time(NULL);
            time_t this_period = now / kRollPerSeconds_ * kRollPerSeconds_;
            if(this_period != start_of_period_) {
                RollFile();
            }
            else if(now - last_flush_ > flush_interval_) {
                last_flush_ = now;
                file_->Flush();
            }
        }
    }
}

bool LogFile::RollFile() {
    time_t now = 0;
    std::string filename = GetLogFileName(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

    if(now > last_roll_) {
        last_roll_ = now;
        last_flush_ = now;
        start_of_period_ = start;
        file_.reset(new FileUtil::AppendFile(filename));
        return true;
    }
    return false;
}

std::string LogFile::GetLogFileName(const std::string& basename, time_t* now) {
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    localtime_r(now, &tm);
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;

    filename += ProcessInfo::Hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::Pid());
    filename += pidbuf;

    filename += ".log";
    
    return filename;
}

} // !namespace ibn

