#include "LobbyManager.h"
#include "JaegerNet.pb.h"
#include "ErrorCodes.h"
#include "Server.h"
#include <atomic>

using namespace google::protobuf;
using namespace JaegerNet;

LobbyManager::LobbyManager()
{

}

LobbyManager::~LobbyManager()
{

}

void LobbyManager::OnMessageReceived(IServer* const sender, MessageReceivedEventArgs& eventArgs) noexcept
{
    JaegerNetResponse response;
    if (eventArgs.Message.has_createlobbyrequest())
    {
        response = HandleCreateLobbyRequest(sender, eventArgs);
    }
    else if (eventArgs.Message.has_connectrequest())
    {
        response = HandleConnectRequest(sender, eventArgs);
    }
    else if (eventArgs.Message.has_controllerinputrequest())
    {
        response = HandleControllerInputRequest(sender, eventArgs);
    }

    response.set_messageid(eventArgs.Message.messageid());
    sender->Send(response);
}

JaegerNetResponse LobbyManager::HandleCreateLobbyRequest(IServer* const /*sender*/, MessageReceivedEventArgs& /*eventArgs*/) noexcept
{
    static std::atomic_int32_t NextLobbyId = 1;

    JaegerNetResponse response;
    auto createLobbyResponse = std::make_unique<CreateLobbyResponse>();

    if (auto lobbyId = NextLobbyId++; lobbyId < MaxLobbies)
    {
        std::unique_lock<std::shared_mutex> lock(m_lobbiesLock);

        createLobbyResponse->set_lobbyid(lobbyId);
        m_lobbies.emplace(lobbyId, lobbyId);
    }
    else
    {
        response.set_error(static_cast<int32>(JaegerNetError::MaxLobbiesExceeded));
    }

    response.set_allocated_createlobbyresponse(createLobbyResponse.release());
    return std::move(response);
}

JaegerNetResponse LobbyManager::HandleConnectRequest(IServer* const sender, MessageReceivedEventArgs& eventArgs) noexcept
{
    std::shared_lock<std::shared_mutex> lock(m_lobbiesLock);

    JaegerNetResponse response;
    auto connectMessageResponse = std::make_unique<ConnectResponse>();
    auto lobbyId = eventArgs.Message.connectrequest().lobbyid();

    if (auto lobbyIter = m_lobbies.find(lobbyId); lobbyIter == m_lobbies.end())
    {
        response.set_error(static_cast<int32>(JaegerNetError::LobbyNotFound));
    }
    else
    {
        try
        {
            auto player = lobbyIter->second.OnConnectRequest(sender, std::move(eventArgs.Endpoint));
            m_playerIdToLobbyIdMap.emplace(lobbyId, player.PlayerId());
        }
        catch (JaegerNetException& ex)
        {
            response.set_error(static_cast<int32>(ex.Error()));
        }
    }

    response.set_allocated_connectresponse(connectMessageResponse.release());
    return std::move(response);
}

JaegerNetResponse LobbyManager::HandleControllerInputRequest(IServer* const /*sender*/, MessageReceivedEventArgs& /*eventArgs*/) noexcept
{
    JaegerNetResponse response;

    auto controllerInputResponse = std::make_unique<ControllerInputResponse>();
    response.set_allocated_controllerinputresponse(controllerInputResponse.release());

    return std::move(response);
}
