#pragma once

namespace JaegerNet
{
    typedef int32_t EventRegistrationToken;

    enum class ControllerDPadButton : uint32_t
    {
        Centered = 0x00,
        Up = 0x01,
        Right = 0x02,
        Down = 0x04,
        Left = 0x08,
    };

    enum class ControllerButton : uint32_t
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
}
