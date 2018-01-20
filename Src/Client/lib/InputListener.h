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
    typedef std::function<void(int controllerIndex, const ControllerState& controllerState)> ControllerStateChangedCallback;
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
        void OnControllerAdded(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent);
        void OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent);
        void OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent);
        void RunInputThread();
        void RunJoystickThread();

        bool m_isRunning = true;
        std::thread m_inputThread;
        std::thread m_joystickThread;

        EventSource<int> m_controllerAddedEventSource;
        EventSource<int> m_controllerRemovedEventSource;
        EventSource<int, ControllerState> m_controllerStateChangedEventSource;

        std::shared_mutex m_controllersLock;
        std::vector<Controller> m_controllers;
    };
}
