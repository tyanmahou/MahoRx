#pragma once
#include <functional>
#include <exception>
#include "IObserver.hpp"
namespace mahorx
{
    template <class T>
    class Subscribe final : public IObserver<T>
    {
    public:
        Subscribe() = default;

        Subscribe(const std::function<void(const T&)>& onNext) :
            m_onNext(onNext)
        {}

        Subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void(std::exception)>& onError
        ) :
            m_onNext(onNext),
            m_onError(onError)
        {}

        Subscribe(
            const std::function<void(const T&)>& onNext,
            const std::function<void()>& onCompleted
        ) :
            m_onNext(onNext),
            m_onCompleted(onCompleted)
        {}

        Subscribe(
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

    constexpr struct Subscribe_Fn
    {
        template<class Func>
        struct ArgsOnNextError;

        template<class Func>
        struct ArgsOnNextCompleted;

        template<class Func>
        struct ArgsOnNextErrorCompleted;

        template<class Func>
        struct ArgsOnNext
        {
            Func onNext;

            ArgsOnNextError<Func> onError(const std::function<void(std::exception)>& onError) const
            {
                return { onNext, onError };
            }

            ArgsOnNextCompleted<Func> onCompleted(const std::function<void()>& onCompleted) const
            {
                return { onNext, onCompleted };
            }
        };
        template<class Func>
        struct ArgsOnNextError
        {
            Func onNext;
            std::function<void(std::exception)> onError;

            ArgsOnNextErrorCompleted<Func> onCompleted(const std::function<void()>& onCompleted) const
            {
                return { onNext, onError, onCompleted };
            }
        };
        template<class Func>
        struct ArgsOnNextCompleted
        {
            Func onNext;
            std::function<void()> onCompleted;
        };
        template<class Func>
        struct ArgsOnNextErrorCompleted
        {
            Func onNext;
            std::function<void(std::exception)> onError;
            std::function<void()> onCompleted;
        };


        template<class Func>
        ArgsOnNext<Func> onNext(const Func& onNext) const
        {
            return { onNext };
        }

        template<class T, class Func>
        friend auto operator | (const T& source, const Subscribe_Fn& fn)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>());
        }

        template<class T, class Func>
        friend auto operator | (const T& source, const ArgsOnNext<Func>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.onNext));
        }
        template<class T, class Func>
        friend auto operator | (const T& source, const ArgsOnNextError<Func>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.onNext, args.onError));
        }
        template<class T, class Func>
        friend auto operator | (const T& source, const ArgsOnNextCompleted<Func>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.onNext, args.onCompleted));
        }
        template<class T, class Func>
        friend auto operator | (const T& source, const ArgsOnNextErrorCompleted<Func>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.onNext, args.onError, args.onCompleted));
        }
    }subscribe;
}