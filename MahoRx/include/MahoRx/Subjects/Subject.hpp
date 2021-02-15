#pragma once
#include <vector>
#include <optional>
#include "ISubject.hpp"
#include "../System/IDisposable.hpp"
#include "../System/Observer.hpp"

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
                if (m_parent.expired()) {
                    return;
                }
                if (m_observer.expired()) [[unlikly]] {
                    return;
                }
                if (auto lockParent = m_parent.lock()) {
                    if (auto lockOvserver = m_observer.lock()) {
                        std::erase(lockParent->m_observers, lockOvserver);
                    }
                    m_parent.reset();
                    m_observer.reset();
                }
            }
        private:
            std::weak_ptr<Subject<T>> m_parent;
            std::weak_ptr<IObserver<T>> m_observer;
        };
    public:
        void onNext(const T& value) override
        {
            for (const auto& observer : m_observers) {
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
            for (const auto& observer : m_observers) {
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
            for (const auto& observer : m_observers) {
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

        std::shared_ptr<IDisposable> subscribe(const std::function<void(const T&)>& onNext)
        {
            return subscribe(std::make_shared<Observer::Subscribe<T>>(onNext));
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError
        ) {
            return subscribe(std::make_shared<Observer::Subscribe<T>>(onNext, onError));
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void()>& onCompleted
        ) {
            return subscribe(std::make_shared<Observer::Subscribe<T>>(onNext, onCompleted));
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError,
            const std::function<void()>& onCompleted
        ) {
            return subscribe(std::make_shared<Observer::Subscribe<T>>(onNext, onError, onCompleted));
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