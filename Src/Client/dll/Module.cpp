#include "JaegerNetClient.h"
#include "Client.h"
#include "InputListener.h"
#include "Lobby.h"

using namespace JaegerNet;

void JaegerNet_StartClient(const char* const hostname, const char* const port) try
{
    JaegerNet::CreateClient(hostname, port);
    JaegerNet::GetClient()->Run(true);
}
JAEGERNET_CATCH_LOG();

void JaegerNet_StopClient(void) try
{
    JaegerNet::DestroyClient();
}
JAEGERNET_CATCH_LOG();

void JaegerNet_StartInputListener(JaegerNet_ControllerAddedCallback controllerAddedCallback, JaegerNet_ControllerRemovedCallback controllerRemovedCallback) try
{
    JaegerNet::CreateInputListener(std::move(controllerAddedCallback), std::move(controllerRemovedCallback));
}
JAEGERNET_CATCH_LOG();

void JaegerNet_StopInputListener(void) try
{
    JaegerNet::DestroyInputListener();
}
JAEGERNET_CATCH_LOG();

void JaegerNet_CreateLobby(JaegerNet_ErrorCallback errorCallback, JaegerNet_LobbyCreatedCallback lobbyCreatedCallback) try
{
    auto client = JaegerNet::GetClient();

    auto createLobbyRequest = std::make_unique<CreateLobbyRequest>();
    JaegerNetRequest request;
    request.set_allocated_createlobbyrequest(createLobbyRequest.release());

    client->Send(request, [errorCallback, lobbyCreatedCallback](const JaegerNetResponse& response)
    {
        auto error = static_cast<JaegerNetError>(response.error());
        auto lobbyId = response.createlobbyresponse().lobbyid();

        errorCallback(error);
        if (error == JaegerNetError::Success)
        {
            lobbyCreatedCallback(lobbyId);
        }
    });
}
JAEGERNET_CATCH_LOG();

void JaegerNet_Connect(int32_t lobbyId, JaegerNet_ErrorCallback errorCallback, JaegerNet_PlayerConnectedCallback playerConnectedCallback, JaegerNet_DisconnectedCallback playerDisconnectedCallback) try
{
    Lobby::Instance().PlayerConnected(std::move(playerConnectedCallback));
    Lobby::Instance().PlayerDisconnected(std::move(playerDisconnectedCallback));

    auto client = JaegerNet::GetClient();

    auto connectRequest = std::make_unique<ConnectRequest>();
    connectRequest->set_lobbyid(lobbyId);

    JaegerNetRequest request;
    request.set_allocated_connectrequest(connectRequest.release());

    client->Send(request, [errorCallback](const JaegerNetResponse& response)
    {
        auto error = static_cast<JaegerNetError>(response.error());
        errorCallback(error);
    });
}
JAEGERNET_CATCH_LOG();

void JaegerNet_Disconnect(int /*controllerIndex*/, JaegerNet_ErrorCallback errorCallback) try
{
    auto client = JaegerNet::GetClient();

    auto disconnectRequest = std::make_unique<DisconnectRequest>();
    //disconnectRequest->set_playerid(playerId);

    JaegerNetRequest request;
    request.set_allocated_disconnectrequest(disconnectRequest.release());

    client->Send(request, [errorCallback](const JaegerNetResponse& response)
    {
        auto error = static_cast<JaegerNetError>(response.error());
        errorCallback(error);
    });
}
JAEGERNET_CATCH_LOG();
