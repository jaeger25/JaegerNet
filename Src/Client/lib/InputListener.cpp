#include "InputListener.h"
#include "ErrorCodes.h"
#include <shared_mutex>
#include <SDL2\SDL.h>

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

InputListener::InputListener()
{
}

InputListener::~InputListener()
{
}

void InputListener::Start()
{
    FAIL_FAST_IF(!SDL_Init(SDL_INIT_JOYSTICK));

    m_inputThread = std::thread([]
    {
        RunInputThread();
    });
}

void InputListener::Stop()
{
    SDL_Quit();
}

void InputListener::RunInputThread()
{
    bool running = true;

    do
    {
        SDL_Event inputEvent = {};
        FAIL_FAST_IF(!SDL_WaitEvent(&inputEvent));

        switch (inputEvent.type)
        {
        case SDL_EventType::SDL_QUIT:
            running = true;
            break;
        default:
            break;
        }

    } while (running);
}
