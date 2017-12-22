#pragma once

namespace JaegerNet
{
    template<typename handle_t>
    class unique_any
    {
    public:
        unique_any() = default;

        unique_any(handle_t* handle, void(*closeFn)(handle_t* handle)) :
            m_handle(handle),
            m_closeFn(closeFn)
        {
        }

        ~unique_any()
        {
            if (m_handle)
            {
                m_closeFn(m_handle);
            }
        }

        handle_t* get() const
        {
            return m_handle;
        }

        void reset(handle_t* handle, void(*closeFn)(handle_t* handle))
        {
            if (m_handle)
            {
                m_closeFn(handle);
            }

            m_handle = handle;
            m_closeFn = closeFn;
        }

    private:
        handle_t* m_handle = nullptr;
        std::function<void(handle_t* handle)> m_closeFn;
    };
}