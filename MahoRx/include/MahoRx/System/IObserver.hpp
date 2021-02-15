#pragma once
#include <exception>

namespace mahorx
{
    /// <summary>
    /// IObserver
    /// </summary>
    template<class T>
    class IObserver
    {
    public:
        virtual ~IObserver() = default;

        virtual void onNext(const T& value) const = 0;
        virtual void onError(std::exception error) const = 0;
        virtual void onCompleted() const = 0;
    };
}