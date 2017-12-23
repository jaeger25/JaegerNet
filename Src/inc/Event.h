#pragma once
#include <atomic>
#include <functional>
#include <map>

namespace JaegerNet
{
    template<typename Arg1, typename... ArgN>
    class EventSource
    {
    public:
        int32_t Add(std::function<void(Arg1, ArgN...)>&& callback)
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

        void Invoke(Arg1 arg1, ArgN... args)
        {
            std::lock_guard<std::mutex> lock(m_callbacksLock);

            for (auto&& callback : m_callbacks)
            {
                callback(arg1, args...);
            }
        }

    private:
        std::mutex m_callbacksLock;
        std::map<int32_t, std::function<void(Arg1, ArgN...)>> m_callbacks;
    };
}
