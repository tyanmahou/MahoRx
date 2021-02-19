#pragma once
#include <memory>
#include <chrono>
#include <functional>
#include"../Fwd.hpp"

namespace mahorx
{
    class IScheduler
    {
    public:

        virtual std::chrono::system_clock::time_point now() const = 0;
        virtual std::shared_ptr<IDisposable> schedule(const std::function<void()>& action) = 0;

        virtual std::shared_ptr<IDisposable> schedule(const std::chrono::seconds& dueTime, const std::function<void()>& action) = 0;
    };
}