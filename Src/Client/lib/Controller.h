#pragma once
#include <memory>
#include <thread>
#include <SDL2\SDL.h>
#include "Resource.h"

namespace JaegerNet
{
    // Values match SDL Hat values
    enum class ControllerDPadButton : uint8_t
    {
        Centered = 0x00,
        Up = 0x01,
        Right = 0x02,
        Down = 0x04,
        Left = 0x08,
    };

    // Order matches SDL button numbers
    enum class ControllerButton : uint16_t
    {
        C_Up = 0x001,
        C_Right = 0x002,
        C_Down = 0x004,
        C_Left = 0x008,
        B = 0x010,
        A = 0x020,
        Trigger_Left = 0x040,
        Trigger_Right = 0x080,
        Trigger_Z = 0x100,
        Start = 0x200,
    };

    struct ControllerState
    {
        int16_t AxisValue;
        ControllerButton ButtonState;
        ControllerDPadButton DPadButtonState;
    };

    class Controller
    {
    public:
        Controller() = default;
        Controller(int controllerIndex);
        ~Controller();

        int32_t InstanceId() const;
        int32_t Index() const;

        const ControllerState CurrentState() const;

        void OnButtonStateChanged(ControllerButton buttonStates, bool pressed);
        void OnDPadButtonStateChanged(ControllerDPadButton buttonStates);
        void OnAxisMotion(int16_t value);

    private:
        int32_t m_instanceId;
        int32_t m_index;
        int16_t m_axisValue = 0;
        uint16_t m_buttonStates = 0;
        uint8_t m_dpadButtonStates = 0;
        unique_any<SDL_Joystick> m_joystick;
    };
}
