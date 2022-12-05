#ifndef IBN_BASE_MUTEX_CONDITION_H
#define IBN_BASE_MUTEX_CONDITION_H

#include <pthread.h>

#include "base/mutex/mutex.h"

namespace ibn {

class Condition : noncopyable {
public:
    explicit Condition(MutexLock& mutex) 
        : mutex_(mutex) {
        pthread_cond_init(&pcond_, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(&pcond_);
    }

    void Wait() {
        MutexLock::UnassignGuard ug(mutex_);
        pthread_cond_wait(&pcond_, mutex_.GetPthreadMutex());
    }

    bool WaitForSeconds(double seconds);

    void Notify() {
        pthread_cond_signal(&pcond_);
    }

    void NotifyAll() {
        pthread_cond_broadcast(&pcond_);
    }

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};

} // !namespace ibn

#endif // !IBN_BASE_MUTEX_CONDITION_H
