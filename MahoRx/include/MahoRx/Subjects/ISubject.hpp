#pragma once

#include "../System/IObservable.hpp"
#include "../System/IObserver.hpp"

namespace mahorx
{
    /// <summary>
    /// ISubject
    /// </summary>
    template<class TSource, class TResult = TSource>
    class ISubject :
        public IObserver<TSource>,
        public IObservable<TResult>
    {
    };
}