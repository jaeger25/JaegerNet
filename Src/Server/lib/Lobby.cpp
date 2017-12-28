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

Player Lobby::OnConnectRequest(IServer* const server, asio::ip::udp::endpoint&& endpoint)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    if (m_players.size() == MaxPlayersPerLobby)
    {
        throw JaegerNetException(JaegerNetError::LobbyCapacityExceeded);
    }

    static int32_t NextPlayerId = 1;

    Player newPlayer(NextPlayerId++, m_players.size() + 1, std::move(endpoint));

    JaegerNetBroadcast broadcast;
    auto connectBroadcast = std::make_unique<ConnectBroadcast>();
    for (auto&& player : m_players)
    {
        connectBroadcast->set_playerid(player.PlayerId());
        connectBroadcast->set_playernumber(player.PlayerNumber());

        broadcast.set_allocated_connectbroadcast(connectBroadcast.get());
        newPlayer.Send(server, broadcast);
    }

    connectBroadcast->set_playerid(newPlayer.PlayerId());
    connectBroadcast->set_playernumber(newPlayer.PlayerNumber());
    m_players.emplace_back(std::move(newPlayer));

    for (auto&& player : m_players)
    {
        player.Send(server, broadcast);
    }

    broadcast.release_connectbroadcast();
    return newPlayer;
}

void Lobby::OnControllerInputRequest(IServer* const /*server*/, const ControllerInputRequest& /*inputRequest*/)
{
    std::shared_lock<std::shared_mutex> lock(m_playersLock);

    JaegerNetBroadcast broadcast;
    auto inputResponse = std::make_unique<ControllerInputResponse>();

    JaegerNetResponse response;
}