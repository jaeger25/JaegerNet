#include "Lobby.h"
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
        if (!m_availableControllerIndexMap.count(controllerIndex))
        {
            m_availableControllerIndexMap[controllerIndex] = true;
        }
    });
}

Lobby::~Lobby()
{
    GetInputListener()->ControllerAdded(m_controllerAddedToken);
}

void Lobby::AddPlayer(int32_t playerId)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    m_players.emplace(playerId, playerId);
}

void Lobby::RemovePlayer(int32_t playerId)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    m_players.erase(playerId);
}
