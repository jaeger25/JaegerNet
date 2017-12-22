#include "InputListener.h"
#include "Controller.h"
#include "ErrorCodes.h"
#include <shared_mutex>

using namespace JaegerNet;

std::shared_mutex InputListenerLock;
std::shared_ptr<IInputListener> InputListenerInstance;

void JaegerNet::CreateInputListener()
{
    std::lock_guard<std::shared_mutex> lock(InputListenerLock);
    InputListenerInstance = std::make_shared<InputListener>();
}

void JaegerNet::DestroyInputListener(void)
{
    std::lock_guard<std::shared_mutex> lock(InputListenerLock);
    InputListenerInstance.reset();
}

std::shared_ptr<IInputListener> JaegerNet::GetInputListener(void) noexcept
{
    std::shared_lock<std::shared_mutex> lock(InputListenerLock);
    return InputListenerInstance;
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
}

InputListener::~InputListener()
{
    Stop();
    m_inputThread.join();
}

void InputListener::Start()
{
    FAIL_FAST_IF(SDL_Init(SDL_INIT_JOYSTICK));

    m_inputThread = std::thread([this]
    {
        RunInputThread();
    });
}

void InputListener::Stop()
{
    SDL_Quit();
}

void InputListener::OnControllerAdded(const SDL_JoyDeviceEvent& deviceEvent)
{
    m_controllers.emplace_back(std::make_unique<Controller>(deviceEvent.which));
}

void InputListener::OnControllerRemoved(const SDL_JoyDeviceEvent& deviceEvent)
{
    m_controllers.erase(std::find_if(m_controllers.begin(), m_controllers.end(), [deviceEvent](auto&& controller)
    {
        return controller->InstanceId() == deviceEvent.which;
    }));
}

void InputListener::OnControllerButtonChanged(const SDL_JoyButtonEvent& buttonEvent)
{
    auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [buttonEvent](auto&& controller)
    {
        return controller->InstanceId() == buttonEvent.which;
    });

    controllerIter->get()->OnButtonStateChanged(SdlButtonToControllerButton(buttonEvent.button), buttonEvent.state == SDL_PRESSED);
}

void InputListener::OnControlerDPadButtonChanged(const SDL_JoyHatEvent& hatEvent)
{
    auto controllerIter = std::find_if(m_controllers.begin(), m_controllers.end(), [hatEvent](auto&& controller)
    {
        return controller->InstanceId() == hatEvent.which;
    });

    //controllerIter->get()->OnButtonStateChanged(SdlButtonToControllerButton(hatEvent), buttonEvent.state == SDL_PRESSED);
}

void InputListener::RunInputThread()
{
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
            break;
        case SDL_EventType::SDL_JOYHATMOTION:
            OnControlerDPadButtonChanged(joystickEvent.jhat);
            break;
        default:
            break;
        }

    } while (running);
}
