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

void Lobby::AddPlayer(int32_t playerId)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    if (m_availableControllerIndices.empty())
    {
        throw JaegerNetException(JaegerNetError::ControllerNotFound);
    }

    auto controllerIndex = m_availableControllerIndices.front();
    m_availableControllerIndices.pop();

    m_players.try_emplace(playerId, playerId, controllerIndex);
}

void Lobby::RemovePlayer(int32_t playerId)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    if (auto iter = m_players.find(playerId); iter != m_players.end())
    {
        m_availableControllerIndices.push(iter->second.ControllerIndex());
        m_players.erase(iter);
    }

    m_players.erase(playerId);
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

        {
            std::unique_lock<std::shared_mutex> lock(m_playersLock);

            auto playerIter = m_players.find(connectBroadcast.playerid());
            if (playerIter == m_players.end())
            {
                Player player;
                player.PlayerId(connectBroadcast.playerid());
                player.PlayerNumber(connectBroadcast.playernumber());

                m_players.emplace(connectBroadcast.playerid(), std::move(player));
            }
            else
            {
                playerIter->second.PlayerNumber(connectBroadcast.playernumber());
            }
        }

        m_playerConnectedEventSource.Invoke(connectBroadcast.playernumber());
    }
}
