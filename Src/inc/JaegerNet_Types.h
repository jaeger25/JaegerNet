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

    inline ControllerButton operator|(ControllerButton a, ControllerButton b)
    {
        return static_cast<ControllerButton>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline ControllerButton operator&(ControllerButton a, ControllerButton b)
    {
        return static_cast<ControllerButton>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline ControllerDPadButton operator|(ControllerDPadButton a, ControllerDPadButton b)
    {
        return static_cast<ControllerDPadButton>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline ControllerDPadButton operator&(ControllerDPadButton a, ControllerDPadButton b)
    {
        return static_cast<ControllerDPadButton>(static_cast<int>(a) & static_cast<int>(b));
    }

    struct ControllerState
    {
        int16_t AxisXValue;
        int16_t AxisYValue;
        ControllerButton ButtonState;
        ControllerDPadButton DPadButtonState;
    };

    inline bool operator==(ControllerState a, ControllerState b)
    {
        return a.AxisXValue == b.AxisXValue &&
            a.AxisYValue == b.AxisYValue &&
            a.ButtonState == b.ButtonState &&
            a.DPadButtonState == b.DPadButtonState;
    }

    inline bool operator!=(ControllerState a, ControllerState b)
    {
        return !(a == b);
    }
}
