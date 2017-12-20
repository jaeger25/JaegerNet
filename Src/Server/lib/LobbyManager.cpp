#include "LobbyManager.h"
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

void LobbyManager::OnMessageReceived(IServer& sender, MessageReceivedEventArgs& eventArgs) noexcept
{
    auto message = eventArgs.Message;
    if (message.has_createlobby())
    {
        static std::atomic_int32_t NextLobbyId = 0;

        auto createLobbyMessage = message.createlobby();
        auto lobbyId = NextLobbyId++;

        if (lobbyId < MaxLobbies)
        {
            std::lock_guard<std::shared_mutex> lock(m_lobbiesLock);

            createLobbyMessage.set_lobbyid(lobbyId);
            m_lobbies.emplace(lobbyId, lobbyId);
        }
        else
        {
            createLobbyMessage.set_error(JaegerErrorToProtobuf(JaegerNetError::MaxLobbiesExceeded));
        }

        sender.Send(createLobbyMessage);
    }
    else if (message.has_connect())
    {
        std::shared_lock<std::shared_mutex> lock(m_lobbiesLock);

        auto connectMessage = message.connect();
        auto lobbyId = connectMessage.lobbyid();

        auto lobbyIter = m_lobbies.find(lobbyId);
        if (lobbyIter == m_lobbies.end())
        {
            connectMessage.set_error(JaegerErrorToProtobuf(JaegerNetError::LobbyNotFound));
        }
        else
        {
            try
            {
                Player player = lobbyIter->second.AddPlayer();
                connectMessage.set_playernumber(player.PlayerNumber());
            }
            catch (JaegerNetException& ex)
            {
                connectMessage.set_error(JaegerErrorToProtobuf(ex.Error()));
            }
        }

        sender.Send(connectMessage);
    }
}