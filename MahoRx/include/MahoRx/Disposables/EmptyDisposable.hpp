#pragma once
#include "IDisposable.hpp"

namespace mahorx
{
    class DisposableEmpty final : public IDisposable
    {
    public:
        static const std::shared_ptr<IDisposable>& Instance()
        {
            static std::shared_ptr<IDisposable> instance = std::make_shared<DisposableEmpty>();
            return instance;
        }
    public:
        void dispose() final
        {}

    };
}