#ifndef IBN_BASE_MUTEX_COUNTDOWNLATCH_H
#define IBN_BASE_MUTEX_COUNTDOWNLATCH_H

#include "base/mutex/condition.h"
#include "base/mutex/mutex.h"

namespace ibn {

class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int count);

    void Wait();
    
    void CountDown();

    int GetCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};

} // !namespace ibn

#endif // !IBN_BASE_MUTEX_COUNTDOWNLATCH_H
