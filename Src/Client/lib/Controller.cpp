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

bool Controller::AreButtonsPressed(ControllerButton buttons) const
{
    return (m_buttonStates & static_cast<uint16_t>(buttons)) != 0;
}

bool Controller::AreDPadButtonsPressed(ControllerDPadButton buttons) const
{
    return (m_dpadButtonStates & static_cast<uint8_t>(buttons)) != 0;
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
