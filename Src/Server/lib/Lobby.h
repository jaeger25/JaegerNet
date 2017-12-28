#pragma once

#include <shared_mutex>
#include "ErrorCodes.h"
#include "Player.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    enum { MaxPlayersPerLobby = 4 };

    class IServer;

    class Lobby
    {
    public:
        Lobby(int id);
        virtual ~Lobby();

        int Id() const;

        Player OnConnectRequest(IServer* const server, asio::ip::udp::endpoint&& endpoint);
        void OnControllerInputRequest(IServer* const server, const ControllerInputRequest& inputRequest);

    private:
        int m_id;

        std::shared_mutex m_playersLock;
        std::vector<Player> m_players;
    };
}
