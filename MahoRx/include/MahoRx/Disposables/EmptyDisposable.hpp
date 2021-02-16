#pragma once
#include "IDisposable.hpp"

namespace mahorx
{
    class DisposableEmpty final : public IDisposable
    {
    public:
        void dispose() final
        {}

    };
}