#include "Controller.h"

using namespace JaegerNet;

Controller::Controller(int controllerIndex)
{
    m_joystick.reset(SDL_JoystickOpen(controllerIndex), SDL_JoystickClose);
    m_instanceId = SDL_JoystickInstanceID(m_joystick.get());
}

Controller::~Controller()
{
}

int32_t Controller::InstanceId() const
{
    return m_instanceId;
}

int32_t Controller::Index() const
{
    return m_index;
}

int16_t Controller::AxisValue() const
{
    return m_axisValue;
}

ControllerButton Controller::ControllerButtonState() const
{
    return static_cast<ControllerButton>(m_buttonStates);
}

ControllerDPadButton Controller::ControllerDPadButtonState() const
{
    return static_cast<ControllerDPadButton>(m_dpadButtonStates);
}

void Controller::OnDPadButtonStateChanged(ControllerDPadButton buttonStates)
{
    m_dpadButtonStates = static_cast<uint8_t>(buttonStates);
}

void Controller::OnButtonStateChanged(ControllerButton button, bool pressed)
{
    if (pressed)
    {
        m_buttonStates |= static_cast<uint16_t>(button);
    }
    else
    {
        m_buttonStates &= ~static_cast<uint16_t>(button);
    }
}

void Controller::OnAxisMotion(int16_t value)
{
    m_axisValue = value;
}
