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
}