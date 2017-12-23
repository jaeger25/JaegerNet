#include "Lobby.h"
#include <atomic>

using namespace JaegerNet;
using namespace std;

Lobby::Lobby(int id) :
    m_id(id)
{

}

Lobby::~Lobby()
{

}

int Lobby::Id() const
{
    return m_id;
}

const Player& Lobby::AddPlayer()
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    if (m_players.size() == MaxPlayersPerLobby)
    {
        throw JaegerNetException(JaegerNetError::LobbyCapacityExceeded);
    }

    static std::atomic_int32_t NextPlayerId = 1;

    m_players.emplace_back(NextPlayerId++, m_players.size() + 1);
    return m_players.back();
}