#pragma once

namespace JaegerNet
{
    template<typename handle_t>
    class unique_any
    {
    public:
        unique_any() = default;

        unique_any(handle_t* handle, std::function<void(handle_t* handle)> closeFn) :
            m_handle(handle),
            m_closeFn(closeFn)
        {
        }

        ~unique_any()
        {
            if (m_handle)
            {
                try
                {
                    m_closeFn(m_handle);
                }
                catch (...) {}
            }
        }

        handle_t* get() const
        {
            return m_handle;
        }

        void reset()
        {
            if (m_handle && m_closeFn)
            {
                m_closeFn(m_handle);
            }

            m_handle = nullptr;
            m_closeFn = nullptr;
        }

        void reset(handle_t* handle, std::function<void(handle_t* handle)> closeFn)
        {
            if (m_handle && m_closeFn)
            {
                m_closeFn(m_handle);
            }

            m_handle = handle;
            m_closeFn = closeFn;
        }

    private:
        handle_t* m_handle = nullptr;
        std::function<void(handle_t* handle)> m_closeFn = nullptr;
    };
}