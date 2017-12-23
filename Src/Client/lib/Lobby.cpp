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

    m_controllerIndexToPlayerIdMap[m_availableControllerIndices.front()] = playerId;
    m_availableControllerIndices.pop();

    m_players.emplace(playerId, playerId);
}

void Lobby::RemovePlayer(int32_t playerId)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    auto iter = std::find(m_controllerIndexToPlayerIdMap.begin(), m_controllerIndexToPlayerIdMap.end(), playerId);
    if (iter != m_controllerIndexToPlayerIdMap.end())
    {
        m_availableControllerIndices.push(iter->first);
        m_controllerIndexToPlayerIdMap.erase(iter);
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

    if (auto iter = m_controllerIndexToPlayerIdMap.find(controller.Index()); iter != m_controllerIndexToPlayerIdMap.end())
    {
        // TODO: send input request
    }
}
