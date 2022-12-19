#include "log/logging.h"
#include "log/timestamp.h"
#include "base/common/current_thread.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

namespace ibn
{

/*
class LoggerImpl
{
 public:
  typedef Logger::LogLevel LogLevel;
  LoggerImpl(LogLevel level, int old_errno, const char* file, int line);
  void finish();

  Timestamp time_;
  LogStream stream_;
  LogLevel level_;
  int line_;
  const char* fullname_;
  const char* basename_;
};
*/

__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno) {
    return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

Logger::LogLevel InitLogLevel() {
    auto option = Logger::INFO;
#ifdef IBN_LOG_TRACE
    option = Logger::TRACE;
#endif

#ifdef IBN_LOG_DEBUG
    option = Logger::DEBUG;
#endif

    return option;
    /* if(::getenv("IBN_LOG_TRACE")) */
    /*     return Logger::TRACE; */
    /* else if(::getenv("IBN_LOG_DEBUG")) */
    /*     return Logger::DEBUG; */
    /* else */
    /*     return Logger::INFO; */
}

Logger::LogLevel g_logLevel = InitLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};

// helper class for known string length at compile time
class T {
public:
    T(const char* str, unsigned len)
        :str_(str),
         len_(len) {
        assert(strlen(str) == len_);
    }

    const char* str_;
    const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v) {
    s.Append(v.str_, v.len_);
    return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
    s.Append(v.data_, v.size_);
    return s;
}

void DefaultOutput(const char* msg, int len) {
    size_t n = fwrite(msg, 1, len, stdout);
    //FIXME check n
    (void)n;
}

void DefaultFlush() {
    fflush(stdout);
}

Logger::OutputFunc g_output = DefaultOutput;
Logger::FlushFunc g_flush = DefaultFlush;

} // !namespace ibn

using namespace ibn;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
    : time_(Timestamp::Now()),
      stream_(),
      level_(level),
      line_(line),
      basename_(file) {
    FormatTime();
    CurrentThread::Tid();
    stream_ << T(CurrentThread::TidString(), CurrentThread::TidStringLength()) << T(" ", 1);
    stream_ << T(LogLevelName[level], 6);
    if(savedErrno != 0) {
        stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
    }
}

void Logger::Impl::FormatTime() {
    int64_t microSecondsSinceEpoch = time_.MicroSecondsSinceEpoch();
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
    int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
    if(seconds != t_lastSecond) {
        t_lastSecond = seconds;
        struct tm tm_time;
        ::localtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

        int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        assert(len == 17); (void)len;
    }

    Fmt us(".%06dZ ", microseconds);
    assert(us.Length() == 9);
    stream_ << T(t_time, 17) << T(us.Data(), 9);
}

void Logger::Impl::Finish() {
    stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line)
    : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : impl_(level, 0, file, line) {
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
    : impl_(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort?FATAL:ERROR, errno, file, line) {}

Logger::~Logger() {
    impl_.Finish();
    const LogStream::Buffer& buf(Stream().GetBuffer());
    g_output(buf.Data(), buf.Length());
    if(impl_.level_ == FATAL) {
        g_flush();
        abort();
    }
}

void Logger::SetLogLevel(Logger::LogLevel level) {
    g_logLevel = level;
}

void Logger::SetOutput(OutputFunc out) {
    g_output = out;
}

void Logger::SetFlush(FlushFunc flush) {
    g_flush = flush;
}
