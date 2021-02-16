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
        struct Args
        {
            Func func;
        };
        template<class Func, class Func2>
        struct Args2
        {
            Func func;
            Func2 func2;
        };
        template<class Func, class Func2, class Func3>
        struct Args3
        {
            Func func;
            Func2 func2;
            Func3 func3;
        };
        template<class Func>
        Args<Func> operator()(const Func& func) const
        {
            return { func };
        }
        template<class Func, class Func2>
        Args2<Func, Func2> operator()(const Func& func, const Func2& func2) const
        {
            return { func, func2};
        }
        template<class Func, class Func2, class Func3>
        Args3<Func, Func2, Func3> operator()(const Func& func, const Func2& func2, const Func3& func3) const
        {
            return { func, func2, func3 };
        }
        template<class T, class Func>
        friend auto operator | (const T& source, const Args<Func>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.func));
        }
        template<class T, class Func, class Func2>
        friend auto operator | (const T& source, const Args2<Func, Func2>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.func, args.func2));
        }
        template<class T, class Func, class Func2, class Func3>
        friend auto operator | (const T& source, const Args3<Func, Func2, Func3>& args)
        {
            return source->subscribe(std::make_shared<Subscribe<T::element_type::out_type>>(args.func, args.func2, args.func3));
        }
    }subscribe;
}