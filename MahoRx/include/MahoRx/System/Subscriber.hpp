#pragma once
#include <functional>
#include <exception>
#include "IObserver.hpp"
namespace mahorx
{
    template <class T>
    class Subscriber final : public IObserver<T>
    {
    public:
        Subscriber() = default;

        Subscriber(const std::function<void(const T&)>& onNext) :
            m_onNext(onNext)
        {}

        Subscriber(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError
        ) :
            m_onNext(onNext),
            m_onError(onError)
        {}

        Subscriber(
            const std::function<void(const T&)>& onNext,
            const std::function<void()>& onCompleted
        ) :
            m_onNext(onNext),
            m_onCompleted(onCompleted)
        {}

        Subscriber(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError,
            const std::function<void()>& onCompleted
        ) :
            m_onNext(onNext),
            m_onError(onError),
            m_onCompleted(onCompleted)
        {}

        void onNext(const T& value) final
        {
            if (m_onNext) {
                m_onNext(value);
            }
        }
        void onError(std::exception error) final
        {
            if (m_onError) {
                m_onError(error);
            }
        }
        void onCompleted() final
        {
            if (m_onCompleted) {
                m_onCompleted();
            }
        }
    private:
        std::function<void(const T&)> m_onNext;
        std::function<void(std::exception)> m_onError;
        std::function<void()> m_onCompleted;
    };
}