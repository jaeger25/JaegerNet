#include "Module.h"
#include "Server.h"
#include "LobbyManager.h"

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    asio::io_service service;

    std::vector<std::unique_ptr<IMessageHandler>> messageHandlers;
    messageHandlers.emplace_back(std::make_unique<LobbyManager>());

    Server server(service, 31337, std::move(messageHandlers));

    service.run();

    return 0;
}
