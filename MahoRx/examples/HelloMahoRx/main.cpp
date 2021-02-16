#include <iostream>
#include <string>
#include <MahoRx.hpp>

int main()
{
    using namespace mahorx;

    auto subject = std::make_shared<Subject<std::string>>();
    subject | subscribe([](const std::string& s) {
        std::cout << s;
    }, []() {
        std::cout << " MahoRx" << std::endl;
    });

    subject->onNext("He");

    auto subscription = subject | subscribe([](const std::string& s) {
        std::cout << s;
    });

    subject->onNext("l");
    subscription->dispose();

    subject->onNext("o");

    subject->onCompleted();
    
    // Hello MahoRx
}
