#pragma once

#include <map>
#include <queue>
#include <shared_mutex>
#include "Client.h"
#include "Event.h"
#include "Player.h"

namespace JaegerNet
{
    typedef std::function<void(int32_t playerNumber)> PlayerConnectedCallback;
    typedef std::function<void(int32_t playerNumber)> PlayerDisconnectedCallback;

    class Controller;
    class BroadcastReceivedEventArgs;

    class Lobby
    {
    public:
        Lobby(Client& client);
        ~Lobby();

        void BindPlayerToController(int32_t playerId, int controllerIndex);

        int32_t PlayerConnected(PlayerConnectedCallback&& callback);
        void PlayerConnected(int32_t token);

        int32_t PlayerDisconnected(PlayerDisconnectedCallback&& callback);
        void PlayerDisconnected(int32_t token);

        void OnBroadcastReceived(const BroadcastReceivedEventArgs& args);
        void OnControllerStateChanged(const Controller& controller);

    private:
        EventSource<int32_t> m_playerConnectedEventSource;
        EventSource<int32_t> m_playerDisconnectedEventSource;

        Client& m_client;

        std::shared_mutex m_playersLock;
        std::map<int, Player> m_players;
        std::map<int, int> m_controllerIndexToPlayerNumberMap;
    };
}
