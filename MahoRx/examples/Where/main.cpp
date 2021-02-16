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
        if (a == 3) {
            throw std::exception("The three");
        }
        return a > 0;
    }) | subscribe([](int a) {
        std::cout << a << std::endl;
    }, [](std::exception ex) {
        std::cout << ex.what() << std::endl;
    }, []() {
        std::cout << "OnComplete" << std::endl;
    });

    subject->onNext(1);
    subject->onNext(2);
    subject->onNext(3);
    subject->onNext(4);

    subject | subscribe([](int a) {
        std::cout << a << std::endl;
    }, []() {
        std::cout << "Complete" << std::endl;
    });
    subject->onNext(5);
    subject->onCompleted();

    // 1, 2
}
