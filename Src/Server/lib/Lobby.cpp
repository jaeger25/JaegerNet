#include "Lobby.h"
#include "Server.h"
#include "JaegerNet.pb.h"

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

const Player& Lobby::OnConnectRequest(IServer* const server, asio::ip::udp::endpoint&& endpoint)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    if (m_players.size() == MaxPlayersPerLobby)
    {
        throw JaegerNetException(JaegerNetError::LobbyCapacityExceeded);
    }

    static int32_t NextPlayerId = 1;

    auto& newPlayer = m_players.emplace_back(NextPlayerId++, m_players.size() + 1, std::move(endpoint));

    auto connectBroadcast = std::make_unique<ConnectBroadcast>();
    connectBroadcast->set_playerid(newPlayer.PlayerId());
    connectBroadcast->set_playernumber(newPlayer.PlayerNumber());

    JaegerNetBroadcast broadcast;
    broadcast.set_allocated_connectbroadcast(connectBroadcast.release());

    for (auto&& player : m_players)
    {
        player.Send(server, broadcast);
    }

    return newPlayer;
}

void Lobby::OnControllerInputRequest(IServer* const /*server*/, const ControllerInputRequest& /*inputRequest*/)
{
    std::shared_lock<std::shared_mutex> lock(m_playersLock);

    JaegerNetBroadcast broadcast;
    auto inputResponse = std::make_unique<ControllerInputResponse>();

    JaegerNetResponse response;
}