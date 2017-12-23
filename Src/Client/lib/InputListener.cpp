#include "InputListener.h"
#include "ErrorCodes.h"
#include <shared_mutex>

using namespace JaegerNet;

constexpr ControllerDPadButton SdlHatToControllerDPadButton(uint8_t button)
{
    return static_cast<ControllerDPadButton>(button);
}

constexpr ControllerButton SdlButtonToControllerButton(uint8_t button)
{
    switch (button)
    {
    case 0:
        return ControllerButton::C_Up;
    case 1:
        return ControllerButton::C_Right;
    case 2:
        return ControllerButton::C_Down;
    case 3:
        return ControllerButton::C_Left;
    case 4:
        return ControllerButton::B;
    case 5:
        return ControllerButton::A;
    case 6:
        return ControllerButton::Trigger_Left;
    case 7:
        return ControllerButton::Trigger_Right;
    case 8:
        return ControllerButton::Trigger_Z;
    case 9:
        return ControllerButton::Start;
    default:
        FAIL_FAST();
        break;
    }
}

InputListener::InputListener(ControllerAddedCallback&& controllerAddedCallback, ControllerRemovedCallback&& controllerRemovedCallback) :
    m_controllerAddedCallback(controllerAddedCallback),
    m_controllerRemovedCallback(controllerRemovedCallback)
{
    m_inputThread = std::thread([this]
    {
        RunInputThread();
    });
}

InputListener::~InputListener()
{
    SDL_Quit();
    m_inputThread.join();
}

const Controller InputListener::GetController(int controllerIndex)
{
    std::shared_lock<std::shared_mutex> lock(m_controllersLock);

    for (auto&& controller : m_controllers)
    {
        if (controller.Index() == controllerIndex)
        {
            return controller;
        }
    }

    throw JaegerNetException(JaegerNetError::ControllerNotFound);
}

int32_t InputListener::ControllerAdded(ControllerAddedCallback&& callback)
{
    for (auto&& controller : m_controllers)
    {
        callback(controller.Index());
    }

    return m_controllerAddedEventSource.Add(std::move(callback));
}

void InputListener::ControllerAdded(int32_t token)
{
    m_controllerAddedEventSource.Remove(token);
}

int32_t InputListener::ControllerRemoved(ControllerRemovedCallback&& callback)
{
    return m_controllerRemovedEventSource.Add(std::move(callback));
}

void InputListener::ControllerRemoved(int32_t token)
{
    m_controllerRemovedEventSource.Remove(token);
}

int32_t InputListener::ControllerStateChanged(ControllerStateChangedCallback&& callback)
{
    return m_controllerStateChangedEventSource.Add(std::move(callback));
}

void InputListener::ControllerStateChanged(int32_t token)
{
    m_controllerStateChangedEventSource.Remove(token);
}

void InputListener::OnControllerAdded(const SDL_JoyDeviceEvent& deviceEvent)
{
    std::unique_lock<std::shared_mutex> lock(m_controllersLock);

    m_controllers.emplace_back(deviceEvent.which);
}

void InputListener::OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent)
{
    std::unique_lock<std::shared_mutex> lock(m_controllersLock);

    m_controllers.erase(std::find_if(m_controllers.begin(), m_controllers.end(), [deviceEvent](auto&& controller)
    {
        return controller.InstanceId() == deviceEvent.which;
    }));
}

void InputListener::OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent)
{
    std::shared_lock<std::shared_mutex> lock(m_controllersLock);

    auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [buttonEvent](auto&& controller)
    {
        return controller.InstanceId() == buttonEvent.which;
    });

    controllerIter->OnButtonStateChanged(SdlButtonToControllerButton(buttonEvent.button), buttonEvent.state == SDL_PRESSED);
}

void InputListener::OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent)
{
    std::shared_lock<std::shared_mutex> lock(m_controllersLock);

    auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [hatEvent](auto&& controller)
    {
        return controller.InstanceId() == hatEvent.which;
    });

    controllerIter->OnDPadButtonStateChanged(SdlHatToControllerDPadButton(hatEvent.value));
}

void InputListener::OnControllerAxisMotion(const SDL_JoyAxisEvent& axisEvent)
{
    std::shared_lock<std::shared_mutex> lock(m_controllersLock);

    auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [axisEvent](auto&& controller)
    {
        return controller.InstanceId() == axisEvent.which;
    });

    controllerIter->OnAxisMotion(axisEvent.value);
}

void InputListener::RunInputThread()
{
    FAIL_FAST_IF(SDL_Init(SDL_INIT_JOYSTICK));

    bool running = true;

    do
    {
        SDL_Event joystickEvent = {};
        FAIL_FAST_IF(!SDL_WaitEvent(&joystickEvent));

        switch (joystickEvent.type)
        {
        case SDL_EventType::SDL_QUIT:
            running = false;
            break;
        case SDL_EventType::SDL_JOYDEVICEADDED:
            OnControllerAdded(joystickEvent.jdevice);
            break;
        case SDL_EventType::SDL_JOYDEVICEREMOVED:
            OnControllerRemoved(joystickEvent.jdevice);
            break;
        case SDL_EventType::SDL_JOYBUTTONDOWN:
        case SDL_EventType::SDL_JOYBUTTONUP:
            OnControllerButtonChanged(joystickEvent.jbutton);
            break;
        case SDL_EventType::SDL_JOYAXISMOTION:
            OnControllerAxisMotion(joystickEvent.jaxis);
            break;
        case SDL_EventType::SDL_JOYHATMOTION:
            OnControlerDPadButtonChanged(joystickEvent.jhat);
            break;
        default:
            break;
        }

    } while (running);
}
