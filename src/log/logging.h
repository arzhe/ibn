#ifndef IBN_LOG_LOGGING_H
#define IBN_LOG_LOGGING_H

#include "log/logstream.h"
#include "log/timestamp.h"

namespace ibn {

class Logger {
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    // compile time calculation of basename of source file
    class SourceFile {
    public:
        template<int N>
        SourceFile(const char (&arr)[N])
            : data_(arr),
              size_(N-1) {
            const char* slash = strrchr(data_, '/'); // builtin function
            if(slash) {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char* filename)
            : data_(filename) {
            const char* slash = strrchr(filename, '/');
            if(slash) {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }

        const char* data_;
        int size_;
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream& Stream() { return impl_.stream_; }

    static LogLevel GetLogLevel();
    static void SetLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static void SetOutput(OutputFunc);
    static void SetFlush(FlushFunc);

private:

    class Impl {
    public:
        typedef Logger::LogLevel LogLevel;
        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
        void FormatTime();
        void Finish();

        Timestamp time_;
        LogStream stream_;
        LogLevel level_;
        int line_;
        SourceFile basename_;
    };

  Impl impl_;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::GetLogLevel() {
    return g_logLevel;
}

//
// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define LOG_TRACE if (ibn::Logger::GetLogLevel() <= ibn::Logger::TRACE) \
    ibn::Logger(__FILE__, __LINE__, ibn::Logger::TRACE, __func__).Stream()
#define LOG_DEBUG if (ibn::Logger::GetLogLevel() <= ibn::Logger::DEBUG) \
    ibn::Logger(__FILE__, __LINE__, ibn::Logger::DEBUG, __func__).Stream()
#define LOG_INFO if (ibn::Logger::GetLogLevel() <= ibn::Logger::INFO) \
    ibn::Logger(__FILE__, __LINE__).Stream()
#define LOG_WARN ibn::Logger(__FILE__, __LINE__, ibn::Logger::WARN).Stream()
#define LOG_ERROR ibn::Logger(__FILE__, __LINE__, ibn::Logger::ERROR).Stream()
#define LOG_FATAL ibn::Logger(__FILE__, __LINE__, ibn::Logger::FATAL).Stream()
#define LOG_SYSERR ibn::Logger(__FILE__, __LINE__, false).Stream()
#define LOG_SYSFATAL ibn::Logger(__FILE__, __LINE__, true).Stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
    ::ibn::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr) {
    if(ptr == NULL) {
        Logger(file, line, Logger::FATAL).Stream() << names;
    }
    return ptr;
}

} // !namespace ibn

#endif // !IBN_LOG_LOGGING_H
