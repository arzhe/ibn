#include <unistd.h>
#include <string>
#include <iostream>

#include "log/logfile.h"
#include "log/logging.h"

std::unique_ptr<ibn::LogFile> g_logfile;

void OutputFunc(const char* msg, int len) {
    g_logfile->Append(msg, len);
}

void FlushFunc() {
    g_logfile->Flush();
}

int main(int argc, char** argv) {
    char name[256] = { '\0' };
    strncpy(name, argv[0], sizeof name - 1);
    g_logfile.reset(new ibn::LogFile(::basename(name), 200*100));
    ibn::Logger::SetOutput(OutputFunc);
    ibn::Logger::SetFlush(FlushFunc);

    std::string line = "This is log : ";
    for(int i = 0; i < 10; ++i) {
        std::cout << line << i << std::endl;
        LOG_INFO << line << i;

        sleep(1);
    }
}

