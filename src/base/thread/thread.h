#ifndef IBN_BASE_THREAD_THREAD_H
#define IBN_BASE_THREAD_THREAD_H

#include <functional>
#include <memory>
#include <pthread.h>

#include "base/thread/atomic.h"
#include "base/mutex/countdownlatch.h"
#include "base/common/types.h"

namespace ibn {

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(ThreadFunc, const std::string& name = std::string());
    // FIXME: make it movable in C++11
        
    ~Thread();

    void Start();
    int Join(); // return pthread_join()

    bool Started() const { return started_; }
    // pthread_t PthreadId() const { return pthread_id_; }
    pid_t Tid() const { return tid_; }
    const std::string& Name() const { return name_; }

    static int NumCreated() { return num_created_.Get(); }

private:
    void SetDefaultName();

    bool started_;
    bool joined_;
    pthread_t pthread_id_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;

    static AtomicInt32 num_created_;
};

} // !namespace ibn

#endif // !IBN_BASE_THREAD_THREAD_H
