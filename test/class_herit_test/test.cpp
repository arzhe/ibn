#include "test.h"

std::shared_ptr<SuperBase> BaseA::Do(std::shared_ptr<Context> c) {
    context = c;
    cout << "BaseA Do" << endl;
    return std::make_shared<BaseB>();
}

std::shared_ptr<SuperBase> BaseB::Do(std::shared_ptr<Context> c) {
    context = c;
    cout << "BaseB Do" << endl;
    return std::make_shared<BaseA>();
}

Context::Context() {
    s = std::make_shared<BaseA>();
    cout << "Context()" << endl;
}

void Context::Handle() {
    s = s->Do(shared_from_this());
}
