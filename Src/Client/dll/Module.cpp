#include "JaegerNetClient.h"
#include "Client.h"
#include "InputListener.h"

using namespace JaegerNet;

void JaegerNet_StartClient(const char* const hostname, const char* const port) try
{
    JaegerNet::CreateClient(hostname, port, {});
    JaegerNet::GetClient()->Run(true);
}
JAEGERNET_CATCH_LOG();

void JaegerNet_StopClient(void) try
{
    JaegerNet::DestroyClient();
}
JAEGERNET_CATCH_LOG();

void JaegerNet_StartInputListener() try
{
    JaegerNet::CreateInputListener();
    JaegerNet::GetInputListener()->Start();
}
JAEGERNET_CATCH_LOG();

void JaegerNet_StopInputListener(void) try
{
    JaegerNet::DestroyInputListener();
}
JAEGERNET_CATCH_LOG();

void JaegerNet_CreateLobby(CreateLobbyCallback callback) try
{
    auto client = JaegerNet::GetClient();

    auto createLobbyRequest = std::make_unique<CreateLobbyRequest>();
    JaegerNetRequest request;
    request.set_allocated_createlobbyrequest(createLobbyRequest.release());

    client->Send(request, [callback](const JaegerNetResponse& response)
    {
        auto error = response.error();
        auto lobbyId = response.createlobbyresponse().lobbyid();
        callback(lobbyId, static_cast<JaegerNetError>(error));
    });
}
JAEGERNET_CATCH_LOG();

void JaegerNet_Connect(int32_t lobbyId, ConnectCallback callback) try
{
    auto client = JaegerNet::GetClient();

    auto connectRequest = std::make_unique<ConnectRequest>();
    connectRequest->set_lobbyid(lobbyId);

    JaegerNetRequest request;
    request.set_allocated_connectrequest(connectRequest.release());

    client->Send(request, [callback](const JaegerNetResponse& response)
    {
        auto error = response.error();
        auto playerNumber = response.connectresponse().playernumber();
        callback(playerNumber, static_cast<JaegerNetError>(error));
    });
}
JAEGERNET_CATCH_LOG();
