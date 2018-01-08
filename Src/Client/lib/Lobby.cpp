#include "Lobby.h"
#include "Client.h"
#include "Controller.h"
#include "JaegerNet_ErrorCodes.h"
#include "InputListener.h"
#include "ControllerInputMessage.pb.h"

using namespace JaegerNet;

Lobby::Lobby(Client& client, InputListener& inputListener) noexcept :
    m_client(client),
    m_inputListener(inputListener)
{
}

Lobby::~Lobby()
{
}

void Lobby::BindPlayerToController(int playerNumber, int controllerIndex)
{
    std::shared_lock<std::shared_mutex> lock(m_playersLock);

    auto playerIter = m_players.find(playerNumber);
    if (playerIter == m_players.end())
    {
        throw JaegerNetException(JaegerNetError::PlayerNotFound);
    }

    playerIter->second.ControllerIndex(controllerIndex);
}

int32_t Lobby::PlayerConnected(PlayerConnectedCallback&& callback)
{
    return m_playerConnectedEventSource.Add(std::move(callback));
}

void Lobby::PlayerConnected(int32_t token)
{
    m_playerConnectedEventSource.Remove(token);
}

int32_t Lobby::PlayerDisconnected(PlayerDisconnectedCallback&& callback)
{
    return m_playerDisconnectedEventSource.Add(std::move(callback));
}

void Lobby::PlayerDisconnected(int32_t token)
{
    m_playerDisconnectedEventSource.Remove(token);
}

void Lobby::OnBroadcastReceived(const BroadcastReceivedEventArgs& args)
{
    if (args.Broadcast.has_connectbroadcast())
    {
        auto connectBroadcast = args.Broadcast.connectbroadcast();
        std::vector<int32_t> newPlayers;

        {
            std::unique_lock<std::shared_mutex> lock(m_playersLock);

            for (auto&& playerInfo : connectBroadcast.playerinfo())
            {
                auto playerIter = m_players.find(playerInfo.playernumber());
                if (playerIter == m_players.end())
                {
                    m_players.try_emplace(playerInfo.playernumber(), playerInfo.playernumber(), m_client, m_inputListener);
                    newPlayers.push_back(playerInfo.playernumber());
                }
            }
        }

        for (auto&& playerNumber : newPlayers)
        {
            m_playerConnectedEventSource.Invoke(playerNumber);
        }
    }
}
