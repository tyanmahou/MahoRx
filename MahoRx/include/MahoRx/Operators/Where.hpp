#pragma once
#include <functional>
#include "OperatorObserverBase.hpp"
#include "OperatorObservableBase.hpp"

namespace mahorx::Operators
{
    template<class T>
    class WhereObservable : 
        public OperatorObservableBase<T>,
        public std::enable_shared_from_this<WhereObservable<T>>
    {
        class Where final : public OperatorObserverBase<T>
        {
        public:
            Where(
                const std::shared_ptr<WhereObservable<T>>& parent,
                const std::shared_ptr<IObserver<T>>& observer,
                const std::shared_ptr<IDisposable>& cancel
            ) :
                OperatorObserverBase<T>(observer, cancel),
                m_prarent(parent)
            {}

            void onNext(const T& value) override
            {
                bool isPassed = false;
                try {
                    isPassed = m_prarent->m_pred(value);
                } catch (std::exception ex) {
                    try {
                        this->m_observer->onError(ex);
                    } catch (...) {}
                    this->dispose();
                    return;
                }

                if (isPassed) {
                    this->m_observer->onNext(value);
                }
            }
            void onError(std::exception error) override
            {
                try {
                    this->m_observer->onError(error);
                } catch (...) {

                }

                this->dispose();
            }
            void onCompleted() override
            {
                try {
                    this->m_observer->onCompleted();
                } catch (...) {

                }

                this->dispose();
            }
        private:
            std::shared_ptr<WhereObservable<T>> m_prarent;
        };

        class WhereWithIndex final : public OperatorObserverBase<T>
        {
        public:
            WhereWithIndex(
                const std::shared_ptr<WhereObservable<T>>& parent,
                const std::shared_ptr<IObserver<T>>& observer,
                const std::shared_ptr<IDisposable>& cancel
            ) :
                OperatorObserverBase<T>(observer, cancel),
                m_prarent(parent)
            {}

            void onNext(const T& value) override
            {
                bool isPassed = false;
                try {
                    isPassed = m_prarent->m_predWithIndex(value, ++m_index);
                } catch (std::exception ex) {
                    try {
                        this->m_observer->onError(ex);
                    } catch (...) {}
                    this->dispose();
                    return;
                }

                if (isPassed) {
                    this->m_observer->onNext(value);
                }
            }
            void onError(std::exception error) override
            {
                try {
                    this->m_observer->onError(error);
                } catch (...) {

                }

                this->dispose();
            }
            void onCompleted() override
            {
                try {
                    this->m_observer->onCompleted();
                } catch (...) {

                }

                this->dispose();
            }
        private:
            std::shared_ptr<WhereObservable<T>> m_prarent;
            size_t m_index = 0;
        };
    public:
        WhereObservable(const std::shared_ptr<IObservable<T>>& source, const std::function<bool(const T&)>& pred):
            m_source(source),
            m_pred(pred)
        {}
        WhereObservable(const std::shared_ptr<IObservable<T>>& source, const std::function<bool(const T&, size_t)>& pred) :
            m_source(source),
            m_predWithIndex(pred)
        {}

        std::shared_ptr<IDisposable> subscribeCore(const std::shared_ptr<IObserver<T>>& observer, const std::shared_ptr<IDisposable>& cancel)
        {
            if (m_pred) {
                return m_source->subscribe(std::make_shared<Where>(this->shared_from_this(), observer, cancel));
            } else {
                return m_source->subscribe(std::make_shared<WhereWithIndex>(this->shared_from_this(), observer, cancel));
            }
        }
    private:
        std::shared_ptr<IObservable<T>> m_source;

        std::function<bool(const T&)> m_pred;
        std::function<bool(const T&, size_t)> m_predWithIndex;
    };
}