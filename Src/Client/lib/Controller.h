#pragma once
#include <memory>
#include <thread>
#include <SDL2\SDL.h>
#include "JaegerNet_Types.h"
#include "Resource.h"

namespace JaegerNet
{
    class Controller
    {
    public:
        Controller() = default;
        Controller(int controllerIndex);
        ~Controller();

        int32_t InstanceId() const;
        int32_t Index() const;

        ControllerState CurrentState() const;
        SDL_Joystick* Joystick() const;

        void OnButtonStateChanged(ControllerButton buttonStates, bool pressed);
        void OnDPadButtonStateChanged(ControllerDPadButton buttonStates);
        void OnAxisXMotion(int16_t value);
        void OnAxisYMotion(int16_t value);

    private:
        int32_t m_instanceId;
        int32_t m_index;
        int16_t m_axisXValue = 0;
        int16_t m_axisYValue = 0;
        uint16_t m_buttonStates = 0;
        uint8_t m_dpadButtonStates = 0;
        unique_any<SDL_Joystick> m_joystick;
    };
}
