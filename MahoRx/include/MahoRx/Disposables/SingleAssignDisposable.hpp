#pragma once
#include <memory>
#include "IDisposable.hpp"

namespace mahorx
{
    class SingleAssignDisposable final : public IDisposable
    {
    public:
        bool isDisposed() const
        {
            return m_isDisposed;
        }

        void setDisposable(const std::shared_ptr<IDisposable>& disposable)
        {
            std::shared_ptr<IDisposable> old = nullptr;

            old = m_current;
            if (!m_isDisposed) {
                if (disposable == nullptr) {
                    return;
                }

                m_current = disposable;
            }
            if (m_isDisposed && disposable != nullptr) {
                disposable->dispose();
                return;
            }
            if (old != nullptr) {
                throw std::exception("Disposable is already set");
            }
        }

        void dispose() final
        {
            m_isDisposed = true;
            if (m_current != nullptr) {
                m_current->dispose();
                m_current = nullptr;
            }
        }
    private:
        std::shared_ptr<IDisposable> m_current;
        bool m_isDisposed;
    };
}