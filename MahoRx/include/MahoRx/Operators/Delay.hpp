#pragma once
#include "../Schedulers/ISchduler.hpp"
#include "OperatorObserverBase.hpp"
#include "OperatorObservableBase.hpp"

namespace mahorx::Operators
{
    template<class T>
    class DelayObservable : 
        public OperatorObservableBase<T>,
        public std::enable_shared_from_this<DelayObservable<T>>
    {
        class Delay : public: OperatorObserverBase<T>
        {
        public:

        private:
        };
    public:
        DelayObservable(
            const std::shared_ptr<IObservable<T>>& source,
            const std::chrono::seconds& dueTime,
            const std::shared_ptr<IObservable<T>>& scheduler
        ):
            m_source(source),
            m_dueTime(dueTime),
            m_scheduler(scheduler)
        {}

        std::shared_ptr<IDisposable> subscribeCore(const std::shared_ptr<IObserver<T>>& observer, const std::shared_ptr<IDisposable>& cancel)
        {
            return std::make_shared<Delay>(this->shared_from_this(), observer, cancel).run();
        }
    private:
        std::shared_ptr<IObservable<T>> m_source;
        std::chrono::seconds m_dueTime;
        std::shared_ptr<IObservable<T>> m_scheduler;
    };
}