#include <iostream>
#include <memory>

using namespace std;

class SuperBase;
class BaseA;
class BaseB;

class Context : public std::enable_shared_from_this<Context> {
public:
    Context();
    ~Context() { cout << "~Context()" << endl; }

    void Handle();

private:
    std::shared_ptr<SuperBase> s;
};

class SuperBase {
public:
    virtual ~SuperBase() {}

    virtual std::shared_ptr<SuperBase> Do(std::shared_ptr<Context> c) = 0;

protected:
    std::weak_ptr<Context> context;
};

class BaseA : public SuperBase {
public:
    BaseA() { cout << "BaseA()" << endl; }
    ~BaseA() { cout << "~BaseA()" << endl; }

    std::shared_ptr<SuperBase> Do(std::shared_ptr<Context> c);

};

class BaseB : public SuperBase {
public:
    BaseB() { cout << "BaseB()" << endl; }
    ~BaseB() { cout << "~BaseB()" << endl; }

    std::shared_ptr<SuperBase> Do(std::shared_ptr<Context> c);
};

