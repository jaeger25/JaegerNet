#pragma once
#include <memory>
#include <thread>
#include <vector>
#include <SDL2\SDL.h>

namespace JaegerNet
{
    class IController;

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
        void RunInputThread();
        void OnControllerAdded(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent);
        void OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent);

        std::thread m_inputThread;
        std::vector<std::unique_ptr<IController>> m_controllers;
    };

    extern void CreateInputListener(void);
    extern void DestroyInputListener(void);
    extern std::shared_ptr<IInputListener> GetInputListener(void) noexcept;
}
