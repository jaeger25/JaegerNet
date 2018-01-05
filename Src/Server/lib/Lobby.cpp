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

EventRegistrationToken Lobby::PlayerConnected(PlayerConnectedCallback&& callback)
{
    std::vector<PlayerConnectedEventArgs> args;

    {
        std::shared_lock<std::shared_mutex> lock(m_playersLock);

        for (auto&& player : m_players)
        {
            args.push_back({ player });
        }
    }

    return m_playerConnectedEventSource.Add(std::move(callback));
}

void Lobby::PlayerConnected(EventRegistrationToken token)
{
    m_playerConnectedEventSource.Remove(token);
}

std::vector<std::shared_ptr<Player>> Lobby::Players()
{
    std::shared_lock<std::shared_mutex> lock(m_playersLock);
    return m_players;
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
    PlayerConnectedEventArgs connectedEventArgs;

    {
        std::unique_lock<std::shared_mutex> lock(m_playersLock);

        if (m_players.size() == MaxPlayersPerLobby)
        {
            throw JaegerNetException(JaegerNetError::LobbyCapacityExceeded);
        }

        m_players.emplace_back(std::make_shared<Player>(static_cast<int>(m_players.size() + 1), m_server, std::move(args.Endpoint)));
        connectedEventArgs.Player = m_players.back();

        auto connectBroadcast = std::make_unique<ConnectBroadcast>();
        for (auto&& player : m_players)
        {
            auto playerInfo = connectBroadcast->add_playerinfo();

            playerInfo->set_playernumber(player->PlayerNumber());
        }

        JaegerNetBroadcast broadcast;
        broadcast.set_allocated_connectbroadcast(connectBroadcast.release());
        for (auto&& player : m_players)
        {
            m_server.Send(player->Endpoint(), broadcast);
        }

        auto connectResponse = std::make_unique<ConnectResponse>();
        connectResponse->set_playernumber(m_players.back()->PlayerNumber());
        args.Response.set_allocated_connectresponse(connectResponse.release());
    }

    m_playerConnectedEventSource.Invoke(connectedEventArgs);
}
