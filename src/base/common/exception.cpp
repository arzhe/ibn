#include "base/common/exception.h"
#include "base/common/current_thread.h"

namespace ibn {

Exception::Exception(std::string msg)
    : message_(std::move(msg)),
      stack_(CurrentThread::StackTrace(false)) {}

} // !namespace ibn
