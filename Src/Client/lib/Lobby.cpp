#include "Lobby.h"
#include "Client.h"
#include "Controller.h"
#include "ErrorCodes.h"
#include "InputListener.h"

using namespace JaegerNet;

Lobby& Lobby::Instance()
{
    static Lobby Instance;
    return Instance;
}

Lobby::Lobby()
{
    m_controllerAddedToken = GetInputListener()->ControllerAdded([this](int controllerIndex)
    {
        OnControllerAdded(controllerIndex);
    });

    m_controllerStateChangedToken = GetInputListener()->ControllerStateChanged([this](const Controller& controller)
    {
        OnControllerStateChanged(controller);
    });

    m_broadcastReceivedToken = GetClient()->BroadcastReceived([this](const BroadcastReceivedEventArgs& args)
    {
        OnBroadcastReceived(args);
    });
}

Lobby::~Lobby()
{
    if (auto inputListener = GetInputListener(); inputListener != nullptr)
    {
        inputListener->ControllerAdded(m_controllerAddedToken);
        inputListener->ControllerStateChanged(m_controllerStateChangedToken);
    }

    if (auto client = GetClient(); client != nullptr)
    {
        client->BroadcastReceived(m_broadcastReceivedToken);
    }
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

void Lobby::OnControllerAdded(int controllerIndex)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);
    m_availableControllerIndices.push(controllerIndex);
}

void Lobby::OnControllerStateChanged(const Controller& controller)
{
    std::shared_lock<std::shared_mutex> lock(m_playersLock);

    for (auto&& player : m_players)
    {
        if (player.second.ControllerIndex() == controller.Index())
        {
            player.second.OnControllerStateChanged(controller);
            break;
        }
    }
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
                auto playerIter = m_players.find(playerInfo.playerid());
                if (playerIter == m_players.end())
                {
                    Player player;
                    player.PlayerId(playerInfo.playerid());
                    player.PlayerNumber(playerInfo.playernumber());

                    m_players.emplace(playerInfo.playerid(), std::move(player));
                    newPlayers.push_back(player.PlayerNumber());
                }
                else
                {
                    if (playerIter->second.PlayerNumber() != playerInfo.playernumber())
                    {
                        playerIter->second.PlayerNumber(playerInfo.playernumber());
                        newPlayers.push_back(playerInfo.playernumber());
                    }
                }
            }
        }

        for (auto&& playerNumber : newPlayers)
        {
            m_playerConnectedEventSource.Invoke(playerNumber);
        }
    }
}
