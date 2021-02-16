#pragma once
#include "../System/IObservable.hpp"
#include "../Disposables/SingleAssignDisposable.hpp"

namespace mahorx::Operators
{
    template<class T>
    class OperatorObservableBase : public IObservable<T>
    {
    public:
        std::shared_ptr<IDisposable> subscribe(const std::shared_ptr<IObserver<T>>& observer) final
        {
            auto subscription = std::make_shared<SingleAssignDisposable>();

            subscription->setDisposable(this->subscribeCore(observer, subscription));
            return subscription;
        }
        virtual std::shared_ptr<IDisposable> subscribeCore(const std::shared_ptr<IObserver<T>>& observer, const std::shared_ptr<IDisposable>& cancel) = 0;
    };
}