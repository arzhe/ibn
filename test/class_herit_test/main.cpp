#include <iostream>

#include "test.h"

int main()
{
    std::shared_ptr<Context> c = std::make_shared<Context>();
    for(int i = 0; i < 3; ++i) {
        cout << "========== Cirucle " << i << "==========" << endl;
        c->Handle();
    }
    return 0;
}

