#include "InputListener.h"
#include "JaegerNet_ErrorCodes.h"
#include "Logging.h"
#include <shared_mutex>

using namespace JaegerNet;
using namespace std::chrono_literals;

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

InputListener::InputListener()
{
    m_inputThread = std::thread([this]
    {
        RunInputThread();
    });

    m_joystickThread = std::thread([this]
    {
        RunJoystickThread();
    });
}

InputListener::~InputListener()
{
    m_isRunning = false;

    SDL_Quit();
    m_inputThread.join();
    m_joystickThread.join();
    m_controllers.clear();
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
    std::vector<int> controllerIndices;

    {
        std::shared_lock<std::shared_mutex> lock(m_controllersLock);

        for (auto&& controller : m_controllers)
        {
            controllerIndices.push_back(controller.Index());
        }
    }

    for (auto&& index : controllerIndices)
    {
        callback(index);
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
    {
        std::unique_lock<std::shared_mutex> lock(m_controllersLock);
        m_controllers.emplace_back(deviceEvent.which);
    }

    m_controllerAddedEventSource.Invoke(deviceEvent.which);
}

void InputListener::OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent)
{
    int controllerIndex = -1;

    {
        std::unique_lock<std::shared_mutex> lock(m_controllersLock);

        auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [deviceEvent](auto&& controller)
        {
            return controller.InstanceId() == deviceEvent.which;
        });

        controllerIndex = controllerIter->Index();

        m_controllers.erase(controllerIter);
    }

    m_controllerRemovedEventSource.Invoke(controllerIndex);
}

void InputListener::OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent)
{
    int controllerIndex = 0;
    ControllerState controllerState;
    ControllerState previousControllerState;

    {
        std::shared_lock<std::shared_mutex> lock(m_controllersLock);

        auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [buttonEvent](auto&& controller)
        {
            return controller.InstanceId() == buttonEvent.which;
        });

        previousControllerState = controllerIter->CurrentState();
        controllerIter->OnButtonStateChanged(SdlButtonToControllerButton(buttonEvent.button), buttonEvent.state == SDL_PRESSED);
        controllerState = controllerIter->CurrentState();
        controllerIndex = controllerIter->Index();
    }

    if (previousControllerState != controllerState)
    {
        m_controllerStateChangedEventSource.Invoke(controllerIndex, controllerState);
    }
}

void InputListener::OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent)
{
    int controllerIndex = 0;
    ControllerState controllerState;
    ControllerState previousControllerState;

    {
        std::shared_lock<std::shared_mutex> lock(m_controllersLock);

        auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [hatEvent](auto&& controller)
        {
            return controller.InstanceId() == hatEvent.which;
        });

        previousControllerState = controllerIter->CurrentState();
        controllerIter->OnDPadButtonStateChanged(SdlHatToControllerDPadButton(hatEvent.value));
        controllerState = controllerIter->CurrentState();
        controllerIndex = controllerIter->Index();
    }

    if (previousControllerState != controllerState)
    {
        m_controllerStateChangedEventSource.Invoke(controllerIndex, controllerState);
    }
}

void InputListener::RunInputThread()
{
    FAIL_FAST_IF(SDL_Init(SDL_INIT_JOYSTICK));

    bool running = true;

    do
    {
        SDL_Event joystickEvent = {};
        if (!SDL_WaitEvent(&joystickEvent))
        {
            // TODO: log
            running = false;
        }

        SPDLOG_TRACE(Logger, "Sdl_JoystickEvent: {}", joystickEvent.type);

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
        case SDL_EventType::SDL_JOYHATMOTION:
            OnControlerDPadButtonChanged(joystickEvent.jhat);
            break;
        default:
            break;
        }

    } while (running);
}

void InputListener::RunJoystickThread()
{
    while (m_isRunning)
    {
        std::vector<std::pair<int, ControllerState>> changedControllers;

        {
            std::shared_lock<std::shared_mutex> lock(m_controllersLock);

            for (auto&& controller : m_controllers)
            {
                auto xAxis = SDL_JoystickGetAxis(controller.Joystick(), 0);
                auto yAxis = SDL_JoystickGetAxis(controller.Joystick(), 1);

                ControllerState previousControllerState = controller.CurrentState();

                constexpr int16_t DeadZone = 4096;
                if (xAxis > -DeadZone && xAxis < DeadZone)
                {
                    xAxis = 0;
                }
                if (yAxis > -DeadZone && yAxis < DeadZone)
                {
                    yAxis = 0;
                }

                controller.OnAxisXMotion(xAxis);
                controller.OnAxisYMotion(yAxis);

                ControllerState controllerState = controller.CurrentState();
                if (previousControllerState != controllerState)
                {
                    changedControllers.emplace_back(controller.Index(), controllerState);
                }
            }
        }

        for (auto&&[index, controllerState] : changedControllers)
        {
            m_controllerStateChangedEventSource.Invoke(index, controllerState);
        }

        std::this_thread::sleep_for(16ms);
    }
}
