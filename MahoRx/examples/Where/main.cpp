#include <iostream>
#include <string>
#include <MahoRx.hpp>

int main()
{
    using mahorx::Subject;

    Subject<int> subject;

    subject.where([](int a) {
        return a > 0;
    }).subscribe([](int a) {
        std::cout << a << std::endl;
    });

    subject.onNext(1);
    subject.onNext(-2);
    subject.onNext(3);
    subject.onNext(-4);
    subject.onCompleted();
    subject.dispose();

    // 1, 3
}
