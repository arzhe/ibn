#include <iostream>
#include <memory>

using namespace std;

class Test {
public:
    static std::shared_ptr<Test> NewTest(const std::string& str);

    void Set(const string& str) { test = str; }
    std::string Get() { return test; }

private:
    std::string test;
};

std::shared_ptr<Test> Test::NewTest(const std::string& str) {
    auto t = std::make_shared<Test>();
    t->Set(str);

    return t;
}

int main() {
    
    auto t = Test::NewTest("hello");

    cout << t->Get() << endl;

    return 0;
};
