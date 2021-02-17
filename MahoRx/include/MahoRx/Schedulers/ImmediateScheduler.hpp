#pragma once
#include <thread>
#include "ISchduler.hpp"
#include "../Disposables/EmptyDisposable.hpp"
namespace mahorx::Schedulers
{
    class ImmediateScheduler : public IScheduler
    {
    public:
        std::chrono::system_clock::time_point now() const override
        {
            return std::chrono::system_clock::now();
        }
        std::shared_ptr<IDisposable> schedule(const std::function<void()>& action) override
        {
            action();
            return DisposableEmpty::Instance();
        }

        std::shared_ptr<IDisposable> schedule(const std::chrono::seconds& dueTime, const std::function<void()>& action) override
        {
            std::this_thread::sleep_for(dueTime);
            action();
            return DisposableEmpty::Instance();
        }

    };

    inline const std::shared_ptr<IScheduler>& Immediate()
    {
        static std::shared_ptr<IScheduler> instance = std::make_shared<ImmediateScheduler>();
        return instance;
    }
}

