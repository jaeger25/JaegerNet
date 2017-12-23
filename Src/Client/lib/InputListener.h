#pragma once
#include <memory>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <SDL2\SDL.h>
#include "Controller.h"
#include "Event.h"

namespace JaegerNet
{
    typedef std::function<void(const Controller& controller)> ControllerStateChangedCallback;
    typedef std::function<void(int controllerIndex)> ControllerAddedCallback;
    typedef std::function<void(int controllerIndex)> ControllerRemovedCallback;

    class IInputListener
    {
    public:
        virtual const Controller GetController(int controllerIndex) = 0;

        virtual int32_t ControllerAdded(ControllerAddedCallback&& callback) = 0;
        virtual void ControllerAdded(int32_t token) = 0;

        virtual int32_t ControllerRemoved(ControllerRemovedCallback&& callback) = 0;
        virtual void ControllerRemoved(int32_t token) = 0;

        virtual int32_t ControllerStateChanged(ControllerStateChangedCallback&& callback) = 0;
        virtual void ControllerStateChanged(int32_t token) = 0;
    };

    class InputListener : public IInputListener
    {
    public:
        InputListener(ControllerAddedCallback&& controllerAddedCallback, ControllerRemovedCallback&& controllerRemovedCallback);
        virtual ~InputListener();

        virtual const Controller GetController(int controllerIndex);

        virtual int32_t ControllerAdded(ControllerAddedCallback&& callback);
        virtual void ControllerAdded(int32_t token);

        virtual int32_t ControllerRemoved(ControllerRemovedCallback&& callback);
        virtual void ControllerRemoved(int32_t token);

        virtual int32_t ControllerStateChanged(ControllerStateChangedCallback&& callback);
        virtual void ControllerStateChanged(int32_t token);

    private:
        void RunInputThread();
        void OnControllerAdded(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent);
        void OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent);
        void OnControllerAxisMotion(const SDL_JoyAxisEvent& axisEvent);

        std::thread m_inputThread;
        ControllerAddedCallback m_controllerAddedCallback;
        ControllerRemovedCallback m_controllerRemovedCallback;

        EventSource<int> m_controllerAddedEventSource;
        EventSource<int> m_controllerRemovedEventSource;
        EventSource<int> m_controllerStateChangedEventSource;

        std::shared_mutex m_controllersLock;
        std::vector<Controller> m_controllers;
    };

    extern void CreateInputListener(ControllerAddedCallback&& controllerAddedCallback, ControllerRemovedCallback&& controllerRemovedCallback);
    extern void DestroyInputListener(void);
    extern std::shared_ptr<IInputListener> GetInputListener(void) noexcept;
}
