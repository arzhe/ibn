#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "log/file_util.h"
#include "log/logging.h"

namespace ibn {

namespace FileUtil {

AppendFile::AppendFile(StringArg filename)
    : fp_(::fopen(filename.c_str(), "ae")),
      written_bytes_(0) {
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof buffer_);
}

AppendFile::~AppendFile() {
    ::fclose(fp_);
}

void AppendFile::Append(const char* logline, const size_t len) {
    size_t written = 0;

    while(written != len) {
        size_t remain = len - written;
        size_t n = Write(logline + written, remain);
        if(n != remain) {
            int err = ferror(fp_);
            if(err) {
                fprintf(stderr, "AppendFile::Append() failed %s\n", strerror_tl(err));
                break;
            }
        }
        written += n;
    }

    written_bytes_ += written;
}

void AppendFile::Flush() {
    ::fflush(fp_);
}

size_t AppendFile::Write(const char* logline, size_t len) {
    return ::fwrite_unlocked(logline, 1, len, fp_);
}

ReadSmallFile::ReadSmallFile(StringArg filename)
    : fd_(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
      err_(0) {
    buf_[0] = '\0';
    if(fd_ < 0) {
        err_ = errno;
    }
}

ReadSmallFile::~ReadSmallFile() {
    if(fd_ >= 0) {
        ::close(fd_);
    }
}

template<typename String>
int ReadSmallFile::ReadToString(int max_size,
                                String* content,
                                int64_t* file_size,
                                int64_t* modify_time,
                                int64_t* create_time) {
    static_assert(sizeof(off_t) == 8, "_FILE_OFFSET_BITS = 64");
    assert(content != NULL);
    int err = err_;
    if(fd_ >= 0) {
        content->clear();

        if(file_size) {
            struct stat statbuf;
            if(::fstat(fd_, &statbuf) == 0) {
                if(S_ISREG(statbuf.st_mode)) {
                    *file_size = statbuf.st_size;
                    content->reserve(static_cast<int>(std::min(implicit_cast<int64_t>(max_size), *file_size)));
                }
                else if(S_ISDIR(statbuf.st_mode)) {
                    err = EISDIR;
                }
                if(modify_time) {
                    *modify_time = statbuf.st_mtime;
                }
                if(create_time) {
                    *create_time = statbuf.st_ctime;
                }
            }
            else {
                err = errno;
            }
        }

        while(content->size() < implicit_cast<size_t>(max_size)) {
            size_t to_read = std::min(implicit_cast<size_t>(max_size) - content->size(), sizeof(buf_));
            ssize_t n = ::read(fd_, buf_, to_read);
            if(n > 0) {
                content->append(buf_, n);
            }
            else {
                if(n < 0) {
                    err = errno;
                }
                break;
            }
        }
    }

    return err;
}

int ReadSmallFile::ReadToBuffer(int* size) {
    int err = err_;
    if(fd_ >= 0) {
        ssize_t n = ::pread(fd_, buf_, sizeof(buf_) - 1, 0);
        if(n >= 0) {
            if(size) {
                *size = static_cast<int>(n);
            }
            buf_[n] = '\0';
        }
        else {
            err = errno;
        }
    }
    return err;
}

template int ReadFile(StringArg filename,
                      int max_size,
                      std::string* content,
                      int64_t*, int64_t*, int64_t*);

template int ReadSmallFile::ReadToString(int max_size,
                                         std::string* content,
                                         int64_t*, int64_t*, int64_t*);

} // !namespace FileUtil

} // !namespace ibn
