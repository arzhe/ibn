#ifndef IBN_LOG_FILE_UTIL_H
#define IBN_LOG_FILE_UTIL_H

#include <sys/types.h> // for off_t

#include "base/common/noncopyable.h"
#include "base/common/string_view.h"

namespace ibn {

namespace FileUtil {

class ReadSmallFile : noncopyable {
public:
    ReadSmallFile(StringArg filename);
    ~ReadSmallFile();

    template<typename String>
    int ReadToString(int max_size,
                     String* content,
                     int64_t* file_size,
                     int64_t* modify_time,
                     int64_t* create_time);

    int ReadToBuffer(int* size);

    const char* Buffer() const { return buf_; }

    static const int kBufferSize = 64 * 1024;

private:
    int fd_;
    int err_;
    char buf_[kBufferSize];
};

// Read the file content, returns errno if error happens.
template<typename String>
int ReadFile(StringArg filename,
             int max_size,
             String* content,
             int64_t* file_size = NULL,
             int64_t* modify_time = NULL,
             int64_t* create_time = NULL) {
    ReadSmallFile file(filename);
    return file.ReadToString(max_size, content, file_size, modify_time, create_time);
}

class AppendFile : noncopyable {
public:
    explicit AppendFile(StringArg filename);

    ~AppendFile();

    void Append(const char* logline, size_t len);

    void Flush();

    off_t WrittenBytes() const { return written_bytes_; }

private:
    size_t Write(const char* logline, size_t len);

    FILE* fp_;
    char buffer_[64 * 1024];
    off_t written_bytes_;
};

} // !namespace FileUtil

} // !namespace ibn

#endif // !IBN_BASE_LOG_UTIL_H
