#ifndef IBN_BASE_COMMON_SINGLETON_H
#define IBN_BASE_COMMON_SINGLETON_H

#include <assert.h>
#include <pthread.h>
#include <stdlib.h> // atexit

namespace ibn {

namespace detail {

template<typename T>
struct has_no_destroy {
    template<typename C> static char test(decltype(std::declval<C>().no_destroy));
    template<typename C> static int32_t test(...);
    const static bool value = sizeof(test<T>(0)) == 1;
};

} // !namespace detail

template<typename T>
class Singleton : noncopyable {
public:
    Singleton() = delete;
    ~Singleton() = delete;

    static T& Instance() {
        pthread_once(&ponce_, &Singleton::Init);
        assert(value_ != NULL);
        return *value_;
    }

private:
    static void Init() {
        value_ = new T();
        if(!detail::has_no_destroy<T>::value) {
            ::atexit(Destroy);
        }
    }

    static void Destroy() {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void) dummy;

        delete value_;
        value_ = NULL;
    }

private:
    static pthread_once_t ponce_;
    static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

} // !namespace ibn

#endif // !IBN_BASE_COMMON_SINGLETON_H
