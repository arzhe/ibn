#include <iostream>

#include "log/logging.h"

int main()
{
    LOG_INFO << "Hello world";
    LOG_INFO << "The next one is:";
    LOG_WARN << "You are not allowed to see!";
    LOG_ERROR << "Wrong!";
    //LOG_FATAL << "Down!";
    LOG_SYSERR << "SYSERR!";
    return 0;
}

