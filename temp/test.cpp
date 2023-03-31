#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

int main()
{
    const char* path = "file";
    auto fstr = fopen(path, "a");
    
    char* buf = new char[1024];
    buf[0] = 'a';
    for(int i = 1; i < 100; ++i) {
        buf[i] = 'x';
    }
    int i = 1;
    while(i != 100) {
        size_t count = fwrite(buf++, 1, 1, fstr);
        ++i;
        fflush(fstr);
        sleep(1);
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

