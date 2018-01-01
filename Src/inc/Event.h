#pragma once
#include <atomic>
#include <functional>
#include <map>
#include <shared_mutex>
#include "NoCopy.h"
#include "JaegerNet_Types.h"

namespace JaegerNet
{
    template<typename... Args>
    class EventSource : NoCopy
    {
    public:
        EventSource() noexcept {}

        EventSource(EventSource&& other) noexcept
        {
            std::unique_lock<std::shared_mutex> lock(other.m_callbacksLock);
            m_callbacks = std::move(other.m_callbacks);
        }

        EventRegistrationToken Add(std::function<void(Args...)>&& callback)
        {
            std::unique_lock<std::shared_mutex> lock(m_callbacksLock);

            static EventRegistrationToken NextToken = 0;

            auto token = NextToken++;
            m_callbacks.emplace(token, std::move(callback));

            return token;
        }

        void Remove(EventRegistrationToken token)
        {
            std::unique_lock<std::shared_mutex> lock(m_callbacksLock);

            m_callbacks.erase(token);
        }

        void Invoke(Args... args)
        {
            std::shared_lock<std::shared_mutex> lock(m_callbacksLock);

            for (auto&& callback : m_callbacks)
            {
                callback.second(args...);
            }
        }

    private:
        std::shared_mutex m_callbacksLock;
        std::map<EventRegistrationToken, std::function<void(Args...)>> m_callbacks;
    };
}
