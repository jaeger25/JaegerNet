#include "Client.h"
#include "InputListener.h"

using namespace JaegerNet;
using namespace std;

class Test : public IMessageHandler
{
public:
    void OnMessageReceived(IClient* const sender, MessageReceivedEventArgs& eventArgs) noexcept
    {
        auto message = eventArgs.Message;
        auto error = message.error();
        (error);

        if (message.has_createlobbyresponse())
        {
            auto createLobbyMessage = message.createlobbyresponse();
            auto lobbyId = createLobbyMessage.lobbyid();

            auto connectMessage = std::make_unique<ConnectRequest>();
            connectMessage->set_lobbyid(lobbyId);

            JaegerNetRequest response;
            response.set_allocated_connectrequest(connectMessage.release());

            sender->Send(response, [](const JaegerNetResponse& /*response*/)
            {
            });
        }
        else
        {
            auto connectMessage = message.connectresponse();
            auto playerNum = connectMessage.playernumber();

            (playerNum);
        }
    }
};

int main(int /*argc*/, char** /*argv*/)
{
    std::vector<std::unique_ptr<IMessageHandler>> messageHandlers;
    messageHandlers.emplace_back(std::make_unique<Test>());

    JaegerNet::CreateClient("127.0.0.1", "31337", std::move(messageHandlers));
    auto client = JaegerNet::GetClient();

    auto createLobbyMessage = std::make_unique<CreateLobbyRequest>();
    JaegerNetRequest message;
    message.set_allocated_createlobbyrequest(createLobbyMessage.release());

    client->Send(message, [](const JaegerNetResponse& /*response*/)
    {
    });

    JaegerNet::CreateInputListener(
        [](int /*controllerIndex*/)
    {

    },
        [](int /*controllerIndex*/)
    {

    });

    client->Run(false);

    return 0;
}
