#pragma once
#include "System/IObservable.hpp"
#include "System/Subscriber.hpp"
#include "Subjects/Subject.hpp"

#include "Operators/Where.hpp"

namespace mahorx
{
    template<class T, template<class> class BaseType = IObservable>
    class Observable
    {
    public:
        template<class Type, class... Args>
        static Observable<T> Create(Args&&... args)
        {
            return Observable<T>(std::make_shared<Type>(std::forward<Args>(args)...));
        }

        template<template<class> class ObservableType, class... Args>
        static Observable<T> Create(Args&&... args)
        {
            return Create<ObservableType<T>, Args...>(std::forward<Args>(args)...);
        }
    public:
        Observable(const std::shared_ptr<BaseType<T>>& source):
            m_source(source)
        {}

        /// <summary>
        /// Subscribe
        /// </summary>
        std::shared_ptr<IDisposable> subscribe() const
        {
            return m_source->subscribe(std::make_shared<Subscriber<T>>());
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext
        ) const {
            return m_source->subscribe(std::make_shared<Subscriber<T>>(onNext));
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError
        ) const {
            return m_source->subscribe(std::make_shared<Subscriber<T>>(onNext, onError));
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void()>& onCompleted
        ) const {
            return m_source->subscribe(std::make_shared<Subscriber<T>>(onNext, onCompleted));
        }
        std::shared_ptr<IDisposable> subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError,
            const std::function<void()>& onCompleted
        ) const {
            return m_source->subscribe(std::make_shared<Subscriber<T>>(onNext, onError, onCompleted));
        }

        /// <summary>
        /// Where
        /// </summary>
        Observable<T> where(const std::function<bool(const T&)>& pred)
        {
            return Observable::Create<Operators::WhereObservable>(m_source, pred);
        }
        Observable<T> where(const std::function<bool(const T&, size_t)>& pred)
        {
            return Observable::Create<Operators::WhereObservable>(m_source, pred);
        }
    protected:
        std::shared_ptr<BaseType<T>> m_source;
    };

    template<class T>
    class Subject final : public Observable<T, SubjectImpl>
    {
    public:
        Subject() :
            Observable<T, SubjectImpl>(std::make_shared<SubjectImpl<T>>())
        {}

        void onNext(const T& value) const
        {
            this->m_source->onNext(value);
        }
        void onError(std::exception error) const
        {
            this->m_source->onError(error);
        }
        void onCompleted() const
        {
            this->m_source->onCompleted();
        }
        void dispose() const
        {
            this->m_source->dispose();
        }
    };
}