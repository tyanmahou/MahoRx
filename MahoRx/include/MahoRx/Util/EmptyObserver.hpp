#pragma once
#include <memory>
#include "../System/IObserver.hpp"

namespace mahorx::Util
{
    template<class T>
    class EmptyObserver final: public IObserver<T>
    {
    public:
        static std::shared_ptr<EmptyObserver> Instance()
        {
            static auto instance = std::make_shared<EmptyObserver>();
            return instance;
        }
    public:
        void onNext([[maybe_unused]]const T& value) override
        {}

        void onError([[maybe_unused]]std::exception error) override
        {}
        void onCompleted() override
        {}
    };
}