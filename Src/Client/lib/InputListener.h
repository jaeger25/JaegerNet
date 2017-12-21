#pragma once
#include <memory>
#include <thread>

namespace JaegerNet
{
    class IInputListener
    {
    public:
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

    class InputListener : public IInputListener
    {
    public:
        InputListener();
        virtual ~InputListener();

        virtual void Start();
        virtual void Stop();

    private:
        static void RunInputThread();

        std::thread m_inputThread;
    };

    extern void CreateInputListener(void);
    extern void DestroyInputListener(void);
    extern std::shared_ptr<IInputListener> GetInputListener(void) noexcept;
}
