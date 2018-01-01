#pragma once
#include <functional>
#include <shared_mutex>
#include <vector>
#include "ErrorCodes.h"
#include "NoCopy.h"
#include "Player.h"
#include "Server.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    enum { MaxPlayersPerLobby = 4 };

    struct PlayerConnectedEventArgs
    {
        std::shared_ptr<Player> Player;
    };

    typedef std::function<void(const PlayerConnectedEventArgs& args)> PlayerConnectedCallback;

    class Lobby : NoCopy
    {
    public:
        Lobby(Server& server);
        ~Lobby();

        EventRegistrationToken PlayerConnected(PlayerConnectedCallback&& callback);
        void PlayerConnected(EventRegistrationToken token);

        std::vector<std::shared_ptr<Player>> Players();

    private:
        void HandleConnectRequest(RequestReceivedEventArgs& args);
        void OnRequestReceived(RequestReceivedEventArgs& args);

        EventSource<PlayerConnectedEventArgs> m_playerConnectedEventSource;
        EventRegistrationToken m_requestReceivedToken;
        Server& m_server;

        std::shared_mutex m_playersLock;
        std::vector<std::shared_ptr<Player>> m_players;
    };
}
