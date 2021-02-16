#include <iostream>
#include <string>
#include <MahoRx.hpp>

int main()
{
    using mahorx::Subject;
    using mahorx::subscribe;
    using mahorx::Operators::where;

    auto subject = std::make_shared<Subject<int>>();

    subject | where([](int a) {
        return a > 0;
    }) | subscribe([](int a) {
        std::cout << a << std::endl;
    }); 

    subject->onNext(1);
    subject->onNext(-1);
    subject->onNext(2);
    subject->onCompleted();

    std::cout << "end" << std::endl;
    // 1, 2
}
