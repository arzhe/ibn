#ifndef IBN_BASE_THREAD_ATOMIC_H
#define IBN_BASE_THREAD_ATOMIC_H

#include <stdint.h>

#include "base/common/noncopyable.h"

namespace ibn {

namespace detail {

template<typename T>
class AtomicIntegerT : noncopyable {
public:
    AtomicIntegerT() : value_(0) {}

    // Uncomment if copying and assignment are needed.
    // 
    /* AtomicIntegerT(const AtomicIntegerT& that) */ 
    /*     : value_(that.Get()) {} */

    /* AtomicIntegerT& operator=(const AtomicIntegerT& that) { */
    /*     GetAndSet(that.Get()); */
    /*     return *this; */
    /* } */

    T Get() {
        // CAS
        /* return __sync_val_compare_and_swap(&value_, 0, 0); */
        return __atomic_load_n(&value_, __ATOMIC_SEQ_CST); // in gcc >= 4.7
        // modern C++ :
        /* return value_.load(std::memory_order_seq_cst); */
    }
 
    T GetAndAdd(T x) {
        // CAS
        /* return __sync_fetch_and_add(&value_, x); */
        return __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST); // in gcc >= 4.7
        // modern C++ :
        /* return value_.fetch_add(std::memory_order_seq_cst); */
    }

    T AddAndGet(T x) {
        return GetAndAdd(x) + x;
    }

    T IncrementAndGet() {
        return AddAndGet(1);
    }
    
    T DecrementAndGet() {
        return AddAndGet(-1);
    }

    void Add(T x) {
        GetAndAdd(x);
    }

    void Increment() {
        IncrementAndGet();
    }

    void Decrement() {
        DecrementAndGet();
    }

    T GetAndSet(T new_value) {
        // CAS
        /* return __sync_lock_test_and_set(&value_, new_value); */
        return __atomic_exchange_n(&value_, new_value, __ATOMIC_SEQ_CST); // in gcc >= 4.7
        // modern C++ :
        /* return value_.exchange(new_value, std::memory_order_seq_cst); */
    }

private:
    volatile T value_;
};

} // !namespace detail

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

} // !namespace ibn

#endif // !IBN_BASE_THREAD_ATOMIC_H
