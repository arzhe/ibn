#ifndef IBN_BASE_THREAD_BOUNDED_BLOCKING_QUEUE_H
#define IBN_BASE_THREAD_BOUNDED_BLOCKING_QUEUE_H

#include <boost/circular_buffer.hpp>
#include <assert.h>

#include "base/mutex/condition.h"
#include "base/mutex/mutex.h"

namespace ibn {

template<typename T>
class BoundedBlockingQueue : noncopyable {
public:
    explicit BoundedBlockingQueue(int max_size)
        : mutex_(),
          not_empty_(mutex_),
          not_full_(mutex_),
          queue_(max_size) {}
    
    void Put(const T& x) {
        MutexLockGuard lock(mutex_);
        while(queue_.full()) {
            not_full_.Wait();
        }
        assert(!queue_.full());
        queue_.push_back(x);
        not_empty_.Notify();
    }

    void Put(T&& x) {
        MutexLockGuard lock(mutex_);
        while(queue_.full()) {
            not_full_.Wait();
        }
        assert(!queue_.full());
        queue_.push_back(std::move(x));
        not_empty_.Notify();
    }

    T Take() {
        MutexLockGuard lock(mutex_);
        while(queue_.empty()) {
            not_empty_.Wait();
        }
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        not_full_.Notify();
        return front;
    }

    bool Empty() const {
        MutexLockGuard lock(mutex_);
        return queue_.empty();
    }

    bool Full() const {
        MutexLockGuard lock(mutex_);
        return queue_.full();
    }

    size_t Size() const {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }

    size_t Capacity() const {
        MutexLockGuard lock(mutex_);
        return queue_.capacity();
    }

private:
    mutable MutexLock mutex_;
    Condition not_empty_;
    Condition not_full_;
    boost::circular_buffer<T> queue_;
};

} // !namespace ibn

#endif // !IBN_BASE_THREAD_BOUNDED_BLOCKING_QUEUE_H
