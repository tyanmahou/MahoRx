#pragma once
#include <memory>

#include "../Disposables/IDisposable.hpp"
#include "../System/IObserver.hpp"
#include "../Util/EmptyObserver.hpp"

namespace mahorx::Operators
{
    template<class T, class TR = T>
    class OperatorObserverBase :
        public IObserver<T>,
        public IDisposable
    {
    public:
        OperatorObserverBase(const std::shared_ptr<IObserver<TR>>& observer, const std::shared_ptr<IDisposable>& canncel):
            m_observer(observer),
            m_canncel(canncel)
        {}

        void dispose() final
        {
            m_observer = Util::EmptyObserver<TR>::Instance();
            if (m_canncel) {
                m_canncel->dispose();
            }
            m_canncel = nullptr;
        }
    protected:
        std::shared_ptr<IObserver<TR>> m_observer;
        std::shared_ptr<IDisposable> m_canncel;
    };
}