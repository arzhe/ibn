#include <iostream>
#include <string>

#define COLOR(x) std::string()"we are good " x)

class A {
public:
    static const std::string a;
};

const std::string A::a = COLOR("team") + "?";

int main()
{
    A* a = new A();
    std::cout << A::a << std::endl;
    return 0;
}

