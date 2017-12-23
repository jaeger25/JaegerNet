#include "Client.h"
#include "InputListener.h"
#include "Lobby.h"

using namespace JaegerNet;

std::shared_mutex InputListenerLock;
std::shared_ptr<IInputListener> InputListenerInstance;

std::shared_mutex ClientLock;
std::shared_ptr<IClient> ClientInstance;

void JaegerNet::CreateInputListener(ControllerAddedCallback&& controllerAddedCallback, ControllerRemovedCallback&& controllerRemovedCallback)
{
    std::unique_lock<std::shared_mutex> lock(InputListenerLock);
    InputListenerInstance = std::make_shared<InputListener>(std::move(controllerAddedCallback), std::move(controllerRemovedCallback));
}

void JaegerNet::DestroyInputListener(void)
{
    std::unique_lock<std::shared_mutex> lock(InputListenerLock);
    InputListenerInstance.reset();
}

std::shared_ptr<IInputListener> JaegerNet::GetInputListener(void) noexcept
{
    std::shared_lock<std::shared_mutex> lock(InputListenerLock);
    return InputListenerInstance;
}

void JaegerNet::CreateClient(const char* const hostname, const char* port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers)
{
    std::unique_lock<std::shared_mutex> lock(ClientLock);
    ClientInstance = std::make_unique<Client>(hostname, port, std::move(messageHandlers));
}

void JaegerNet::DestroyClient(void)
{
    std::unique_lock<std::shared_mutex> lock(ClientLock);
    ClientInstance.reset();
}

std::shared_ptr<IClient> JaegerNet::GetClient(void) noexcept
{
    std::shared_lock<std::shared_mutex> lock(ClientLock);
    return ClientInstance;;
}
