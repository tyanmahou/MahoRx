#pragma once
#include <memory>
#include "../Fwd.hpp"

namespace mahorx
{
    /// <summary>
    /// IObservable
    /// </summary>
    template<class T>
    class IObservable
    {
    public:
        virtual ~IObservable() = default;

        virtual std::shared_ptr<IDisposable> subscribe(const std::shared_ptr<IObserver<T>>& observer) = 0;
    };
}