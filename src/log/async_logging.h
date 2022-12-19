#ifndef IBN_LOG_ASYNC_LOGGING_H
#define IBN_LOG_ASYNC_LOGGING_H

#include <atomic>
#include <vector>

#include "base/thread/blocking_queue.h"
#include "base/thread/bounded_blocking_queue.h"
#include "base/mutex/countdownlatch.h"
#include "base/mutex/mutex.h"
#include "base/thread/thread.h"
#include "log/logstream.h"

namespace ibn {

class AsyncLogging : noncopyable {
public:
    AsyncLogging(const std::string& basename,
                 off_t roll_size,
                 int flush_interval = 3);

    ~AsyncLogging() {
        if(running_) {
            Stop();
        }
    }

    void Append(const char* logline, int len);

    void Start() {
        running_ = true;
        thread_.Start();
        latch_.Wait();
    }

    void Stop() {
        running_ = false;
        cond_.Notify();
        thread_.Join();
    }

private:
    void ThreadFunc();

    typedef detail::FixedBuffer<detail::kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flush_interval_;
    std::atomic<bool> running_;
    const std::string basename_;
    const off_t roll_size_;
    Thread thread_;
    CountDownLatch latch_;
    MutexLock mutex_;
    Condition cond_;
    BufferPtr current_buffer_;
    BufferPtr next_buffer_;
    BufferVector buffers_;
};

} // !namespace ibn

#endif // !IBN_LOG_ASYNC_LOGGING_H
