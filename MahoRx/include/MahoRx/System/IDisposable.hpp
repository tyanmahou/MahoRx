#pragma once

namespace mahorx
{
    /// <summary>
    /// IDisposable
    /// </summary>
    class IDisposable
    {
    public:
        virtual~IDisposable() = default;
        virtual void dispose() = 0;
    };

    class DisposableEmpty final : public IDisposable
    {
    public:
        void dispose() final
        {}

    };
}