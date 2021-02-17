#pragma once
#include <vector>
#include <optional>
#include "ISubject.hpp"
#include "../Disposables/EmptyDisposable.hpp"
#include "../System/Subscriber.hpp"

namespace mahorx
{
    template<class T>
    class Subject final : 
        public ISubject<T>,
        public IDisposable,
        public std::enable_shared_from_this<Subject<T>>
    {
    public:
        class Subscription : public IDisposable
        {
        public:
            Subscription(const std::shared_ptr<Subject<T>>& parent, const std::shared_ptr<IObserver<T>>& observer) :
                m_parent(parent),
                m_observer(observer)
            {}

            void dispose() override
            {
                if (!m_parent) {
                    return;
                }
                if (!m_observer) [[unlikely]] {
                    return;
                }
                for (auto it = m_parent->m_observers.begin(); it != m_parent->m_observers.end(); ++it) {
                    if (*it == m_observer) {
                        m_parent->m_observers.erase(it);
                        break;
                    }
                }
            }
        private:
            std::shared_ptr<Subject<T>> m_parent;
            std::shared_ptr<IObserver<T>> m_observer;
        };
    public:
        void onNext(const T& value) override
        {
            auto old = m_observers;
            for (const auto& observer : old) {
                observer->onNext(value);
            }
        }
        void onError(std::exception error)  override
        {
            throwIfDisposed();
            if (m_isStopped) {
                return;
            }
            m_isStopped = true;
            m_error = error;
            auto old = m_observers;
            for (const auto& observer : old) {
                observer->onError(error);
            }
            m_observers.clear();
        }
        void onCompleted() override
        {
            throwIfDisposed();
            if (m_isStopped) {
                return;
            }
            m_isStopped = true;
            auto old = m_observers;
            for (const auto& observer : old) {
                observer->onCompleted();
            }
            m_observers.clear();
        }

        std::shared_ptr<IDisposable> subscribe(const std::shared_ptr<IObserver<T>>& observer) override
        {
            if (!observer) {
                throw std::exception("observer is null");
            }
            throwIfDisposed();
            if (!m_isStopped) {
                m_observers.push_back(observer);
                return std::make_shared<Subscription>(this->shared_from_this(), observer);
            }
            if (m_error) {
                for (const auto& observer : m_observers) {
                    observer->onError(*m_error);
                }
            } else {
                for (const auto& observer : m_observers) {
                    observer->onCompleted();
                }
            }

            return std::make_shared<DisposableEmpty>();
        }

        void dispose() final
        {
            m_isDisposed = true;
            m_observers.clear();
        }
    private:
        void throwIfDisposed()
        {
            if (m_isDisposed) {
                throw std::exception("already disposed");
            }
        }
    private:
        std::optional<std::exception> m_error;
        std::vector<std::shared_ptr<IObserver<T>>> m_observers;
        bool m_isStopped = false;
        bool m_isDisposed = false;
    };
}