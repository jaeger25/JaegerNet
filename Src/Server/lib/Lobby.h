#pragma once

#include <shared_mutex>
#include "ErrorCodes.h"
#include "Player.h"
#include "Server.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    enum { MaxPlayersPerLobby = 4 };

    class Lobby
    {
    public:
        Lobby(Server& server);
        virtual ~Lobby();

    private:
        void HandleConnectRequest(RequestReceivedEventArgs& args);
        void OnRequestReceived(RequestReceivedEventArgs& args);

        EventRegistrationToken m_requestReceivedToken;

        Server& m_server;

        std::shared_mutex m_playersLock;
        std::vector<Player> m_players;
    };
}
