#ifndef IBN_BASE_COMMON_NONCOPYABLE_H
#define IBN_BASE_COMMON_NONCOPYABLE_H

namespace ibn {

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

} // !namespace ibn

#endif // !IBN_BASE_COMMON_NONCOPYABLE_H
