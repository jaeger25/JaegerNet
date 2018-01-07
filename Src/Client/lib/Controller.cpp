#include "Controller.h"

using namespace JaegerNet;

Controller::Controller(int controllerIndex) :
    m_index(controllerIndex)
{
    m_joystick.reset(SDL_JoystickOpen(controllerIndex), [this](SDL_Joystick* joystick)
    {
        if (SDL_JoystickFromInstanceID(m_instanceId))
        {
            SDL_JoystickClose(joystick);
        }
    });
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

const ControllerState Controller::CurrentState() const
{
    ControllerState state;
    state.AxisXValue = m_axisXValue;
    state.AxisYValue = m_axisYValue;
    state.ButtonState = static_cast<ControllerButton>(m_buttonStates);
    state.DPadButtonState = static_cast<ControllerDPadButton>(m_dpadButtonStates);

    return state;
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

void Controller::OnAxisXMotion(int16_t value)
{
    m_axisXValue = value;
}

void Controller::OnAxisYMotion(int16_t value)
{
    m_axisYValue = value;
}