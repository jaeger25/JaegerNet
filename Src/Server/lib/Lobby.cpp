#include "Lobby.h"
#include "Server.h"
#include "ConnectMessage.pb.h"
#include "JaegerNet.pb.h"

using namespace JaegerNet;
using namespace std;

Lobby::Lobby(Server& server) :
    m_server(server)
{
    m_requestReceivedToken = m_server.RequestReceived([this](RequestReceivedEventArgs& args)
    {
        OnRequestReceived(args);
    });
}

Lobby::~Lobby()
{
    m_server.RequestReceived(m_requestReceivedToken);
}

void Lobby::OnRequestReceived(RequestReceivedEventArgs& args)
{
    if (args.Request.has_connectrequest())
    {
        HandleConnectRequest(args);
    }
}

void Lobby::HandleConnectRequest(RequestReceivedEventArgs& args)
{
    std::unique_lock<std::shared_mutex> lock(m_playersLock);

    if (m_players.size() == MaxPlayersPerLobby)
    {
        throw JaegerNetException(JaegerNetError::LobbyCapacityExceeded);
    }

    m_players.emplace_back(m_players.size() + 1, std::move(args.Endpoint));

    auto connectBroadcast = std::make_unique<ConnectBroadcast>();
    for (auto&& player : m_players)
    {
        auto playerInfo = connectBroadcast->add_playerinfo();

        playerInfo->set_playernumber(player.PlayerNumber());
    }

    JaegerNetBroadcast broadcast;
    broadcast.set_allocated_connectbroadcast(connectBroadcast.release());
    for (auto&& player : m_players)
    {
        m_server.Send(player.Endpoint(), broadcast);
    }

    auto connectResponse = std::make_unique<ConnectResponse>();
    connectResponse->set_playernumber(m_players.back().PlayerNumber());
    args.Response.set_allocated_connectresponse(connectResponse.release());
}
