#include "Module.h"
#include "Client.h"

using namespace JaegerNet;
using namespace std;

class Test : public IMessageHandler
{
public:
    void OnMessageReceived(IClient* const sender, MessageReceivedEventArgs& eventArgs) noexcept
    {
        auto message = eventArgs.Message;
        if (message.has_createlobbyresponse())
        {
            auto createLobbyMessage = message.createlobbyresponse();
            auto lobbyId = createLobbyMessage.lobbyid();
            auto error = createLobbyMessage.error();
            (error);

            auto connectMessage = std::make_unique<ConnectRequest>();
            connectMessage->set_lobbyid(lobbyId);

            JaegerNetRequest response;
            response.set_allocated_connectrequest(connectMessage.release());

            sender->Send(response);
        }
        else
        {
            auto connectMessage = message.connectresponse();
            auto playerNum = connectMessage.playernumber();
            auto error = connectMessage.error();

            (playerNum);
            (error);
        }
    }
};

int main(int /*argc*/, char** /*argv*/)
{
    asio::io_service service;

    std::vector<std::unique_ptr<IMessageHandler>> messageHandlers;
    messageHandlers.emplace_back(std::make_unique<Test>());

    Client client(service, "127.0.0.1", "31337", std::move(messageHandlers));

    auto createLobbyMessage = std::make_unique<CreateLobbyRequest>();
    JaegerNetRequest message;
    message.set_allocated_createlobbyrequest(createLobbyMessage.release());

    client.Send(message);

    service.run();

    return 0;
}
