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

    class InputListener
    {
    public:
        InputListener();
        ~InputListener();

        const Controller GetController(int controllerIndex);

        int32_t ControllerAdded(ControllerAddedCallback&& callback);
        void ControllerAdded(int32_t token);

        int32_t ControllerRemoved(ControllerRemovedCallback&& callback);
        void ControllerRemoved(int32_t token);

        int32_t ControllerStateChanged(ControllerStateChangedCallback&& callback);
        void ControllerStateChanged(int32_t token);

    private:
        void RunInputThread();
        void OnControllerAdded(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent);
        void OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent);
        void OnControllerAxisMotion(const SDL_JoyAxisEvent& axisEvent);

        std::thread m_inputThread;

        EventSource<int> m_controllerAddedEventSource;
        EventSource<int> m_controllerRemovedEventSource;
        EventSource<const Controller&> m_controllerStateChangedEventSource;

        std::shared_mutex m_controllersLock;
        std::vector<Controller> m_controllers;
    };
}
