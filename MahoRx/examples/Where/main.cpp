#include <iostream>
#include <string>
#include <MahoRx.hpp>

int main()
{
    auto subject = std::make_shared<mahorx::Subject<int>>();
    subject
        | mahorx::Operators::where([](int a) {
        return a > 0;
    })
        | mahorx::subscribe([](int a) {
        std::cout << a << std::endl;
    });

    subject->onNext(1);
    subject->onNext(-1);
    subject->onNext(2);
    subject->onCompleted();

    // Hello MahoRx
}
