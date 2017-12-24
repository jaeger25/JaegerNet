#pragma once
#include <atomic>
#include <functional>
#include <map>

namespace JaegerNet
{
    template<typename... Args>
    class EventSource
    {
    public:
        int32_t Add(std::function<void(Args...)>&& callback)
        {
            std::lock_guard<std::mutex> lock(m_callbacksLock);

            static int32_t NextToken = 0;

            auto token = NextToken++;
            m_callbacks.emplace(token, std::move(callback));

            return token;
        }

        void Remove(int32_t token)
        {
            std::lock_guard<std::mutex> lock(m_callbacksLock);

            m_callbacks.erase(token);
        }

        void Invoke(Args... args)
        {
            std::lock_guard<std::mutex> lock(m_callbacksLock);

            for (auto&& callback : m_callbacks)
            {
                callback.second(args...);
            }
        }

    private:
        std::mutex m_callbacksLock;
        std::map<int32_t, std::function<void(Args...)>> m_callbacks;
    };
}
