#include <algorithm>
#include <limits>
#include <type_traits>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "log/logstream.h"
#include "base/util/util.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

using namespace ibn;
using namespace ibn::detail;

namespace ibn {

namespace detail {

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong number of digits");

template<typename T>
size_t convert(char buf[], T value) {
    T i = value;
    char* p = buf;

    do {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while(i != 0);

    if(value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

} // !namespace detail

// template class FixedBuffer<kSmallBuffer>;
// template class FixedBuffer<kLargeBuffer>;

} // !namespace ibn

/* template<int SIZE> */
/* const char* FixedBuffer<SIZE>::debugString() { */
/*     *cur_ = '\0'; */
/*     return data_; */
/* } */

/* template<int SIZE> */
/* void FixedBuffer<SIZE>::cookieStart() {} */

/* template<int SIZE> */
/* void FixedBuffer<SIZE>::cookieEnd() {} */

void LogStream::StaticCheck() {
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
                 "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
                 "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10,
                 "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10,
                 "kMaxNumericSize is large enough");
}

template<typename T>
void LogStream::FormatInteger(T v) {
    if(buffer_.Avail() >= kMaxNumericSize) {
        size_t len = convert(buffer_.Current(), v);
        buffer_.Add(len);
    }
}

LogStream& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v) {
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) {
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) {
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) {
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) {
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) {
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(const void* p) {
    /* uintptr_t v = reinterpret_cast<uintptr_t>(p); */
    /* if(buffer_.avail() >= kMaxNumericSize) { */
    /*     char* buf = buffer_.current(); */
    /*     buf[0] = '0'; */
    /*     buf[1] = 'x'; */
    /*     size_t len = convertHex(buf+2, v); */
    /*     buffer_.add(len+2); */
    /* } */
    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if(buffer_.Avail() >= kMaxNumericSize) {
        int len = snprintf(buffer_.Current(), kMaxNumericSize, "%.12g", v);
        buffer_.Add(len);
    }
    return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt, T val) {
    static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type");

    length_ = snprintf(buf_, sizeof buf_, fmt, val);
    assert(static_cast<size_t>(length_) < sizeof buf_);
}

// Explicit instantiations

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);
