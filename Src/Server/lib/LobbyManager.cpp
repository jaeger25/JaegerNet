#include "LobbyManager.h"
#include "JaegerNet.pb.h"
#include "ErrorCodes.h"
#include "Server.h"
#include <atomic>

using namespace JaegerNet;
using namespace std;

LobbyManager::LobbyManager()
{

}

LobbyManager::~LobbyManager()
{

}

void LobbyManager::OnMessageReceived(IServer* const sender, MessageReceivedEventArgs& eventArgs) noexcept
{
    if (eventArgs.Message.has_createlobbyrequest())
    {
        HandleCreateLobbyRequest(sender, eventArgs);
    }
    else if (eventArgs.Message.has_connectrequest())
    {
        HandleConnectRequest(sender, eventArgs);
    }
}

void LobbyManager::HandleCreateLobbyRequest(IServer* const sender, MessageReceivedEventArgs& /*eventArgs*/) noexcept
{
    static std::atomic_int32_t NextLobbyId = 0;

    auto createLobbyResponse = std::make_unique<CreateLobbyResponse>();

    if (auto lobbyId = NextLobbyId++; lobbyId < MaxLobbies)
    {
        std::lock_guard<std::shared_mutex> lock(m_lobbiesLock);

        createLobbyResponse->set_lobbyid(lobbyId);
        m_lobbies.emplace(lobbyId, lobbyId);
    }
    else
    {
        createLobbyResponse->set_error(JaegerErrorToProtobuf(JaegerNetError::MaxLobbiesExceeded));
    }

    JaegerNetResponse response;
    response.set_allocated_createlobbyresponse(createLobbyResponse.release());
    sender->Send(response);
}

void LobbyManager::HandleConnectRequest(IServer* const sender, MessageReceivedEventArgs& eventArgs) noexcept
{
    std::shared_lock<std::shared_mutex> lock(m_lobbiesLock);

    auto connectMessageResponse = std::make_unique<ConnectResponse>();
    auto lobbyId = eventArgs.Message.connectrequest().lobbyid();

    if (auto lobbyIter = m_lobbies.find(lobbyId); lobbyIter == m_lobbies.end())
    {
        connectMessageResponse->set_error(JaegerErrorToProtobuf(JaegerNetError::LobbyNotFound));
    }
    else
    {
        try
        {
            Player player = lobbyIter->second.AddPlayer();
            connectMessageResponse->set_playernumber(player.PlayerNumber());
        }
        catch (JaegerNetException& ex)
        {
            connectMessageResponse->set_error(JaegerErrorToProtobuf(ex.Error()));
        }
    }

    JaegerNetResponse response;
    response.set_allocated_connectresponse(connectMessageResponse.release());

    sender->Send(response);
}
