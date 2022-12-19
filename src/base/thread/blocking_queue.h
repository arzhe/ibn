#ifndef IBN_BASE_THREAD_BLOCKING_QUEUE_H
#define IBN_BASE_THREAD_BLOCKING_QUEUE_H

#include <deque>
#include <assert.h>

#include "base/mutex/condition.h"
#include "base/mutex/mutex.h"

namespace ibn {

template<typename T>
class BlockingQueue : noncopyable {
public:
    using queue_type = std::deque<T>;

    BlockingQueue()
        : mutex_(),
          not_empty_(mutex_),
          queue_() {}

    void Put(const T& x) {
        MutexLockGuard lock(mutex_);
        queue_.push_back(std::move(x));
        not_empty_.Notify();
    }

    void Put(T&& x) {
        MutexLockGuard lock(mutex_);
        queue_.push_back(std::move(x));
        not_empty_.Notify();
    }

    T Take() {
        MutexLockGuard lock(mutex_);
        // Always use a while-loop, due to spurious wakeup
        while(queue_.empty()) {
            not_empty_.Wait();
        }
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return front;
    }

    queue_type Drain() {
        std::deque<T> queue;
        {
            MutexLockGuard lock(mutex_);
            queue = std::move(queue_);
            assert(queue_.empty());
        }
        return queue;
    }
    
    size_t Size() const {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }
    
private:
    mutable MutexLock mutex_;
    Condition not_empty_;
    queue_type queue_;
};

} // !namespace ibn

#endif // !IBN_BASE_THREAD_BLOCKING_QUEUE_H
