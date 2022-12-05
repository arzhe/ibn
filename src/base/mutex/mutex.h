#ifndef IBN_BASE_MUTEX_H
#define IBN_BASE_MUTEX_H

#include <assert.h>
#include <pthread.h>

#include "base/common/current_thread.h"
#include "base/common/noncopyable.h"

namespace ibn {

class MutexLock : noncopyable {
public:
    MutexLock() : holder_(0) {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock() {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool IsLockedByThisThread() const {
        return holder_ == CurrentThread::Tid();
    }

    void AssertLocked() {
        assert(IsLockedByThisThread());
    }

    void Lock() {
        pthread_mutex_lock(&mutex_);
        AssignHolder();
    }

    void Unlock() {
        UnassignHoler();
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* GetPthreadMutex() {
        return &mutex_;
    }

private:
    friend class Condition;

    class UnassignGuard : noncopyable {
    public:
        explicit UnassignGuard(MutexLock& owner)
            : owner_(owner) {
            owner_.UnassignHoler();
        }

        ~UnassignGuard() {
            owner_.AssignHolder();
        }

    private:
        MutexLock& owner_;
    };

    void UnassignHoler() {
        holder_ = 0;
    }

    void AssignHolder() {
        holder_ = CurrentThread::Tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;
};

class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(MutexLock& mutex)
        : mutex_(mutex) {
        mutex_.Lock();
    }

    ~MutexLockGuard() {
        mutex_.Unlock();
    }

private:
    MutexLock& mutex_;
};

} // !namespace ibn

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif // !IBN_BASE_MUTEX_H
