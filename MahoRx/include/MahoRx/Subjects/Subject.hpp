#pragma once
#include <vector>
#include "ISubject.hpp"

namespace mahorx
{
    template<class T>
    class Subject : public ISubject<T>
    {
    public:
        void onNext(const T& value) const override
        {
            if (m_isStopped) {
                return;
            }
            for (const auto& observer : m_observers) {
                observer->onNext(value);
            }
        }
        void onError(std::exception error) const override
        {
            if (m_isStopped) {
                return;
            }
            m_error = error;
            for (const auto& observer : m_observers) {
                observer->onError(error);
            }
        }
    private:
        std::exception m_error;
        std::vector<std::shared_ptr<IObserver<T>>> m_observers;
        bool m_isStopped = false;
    };
}