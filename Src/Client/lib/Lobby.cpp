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
}

Lobby::~Lobby()
{
    GetInputListener()->ControllerAdded(m_controllerAddedToken);
    GetInputListener()->ControllerStateChanged(m_controllerStateChangedToken);
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
