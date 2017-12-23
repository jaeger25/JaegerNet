#pragma once

#include <map>
#include <shared_mutex>
#include <vector>
#include "Player.h"

namespace JaegerNet
{
    class Lobby
    {
    public:
        static Lobby& Instance();

        ~Lobby();
        void AddPlayer(int32_t playerId);
        void RemovePlayer(int32_t playerId);

    private:
        Lobby();

        int32_t m_controllerAddedToken;

        std::shared_mutex m_playersLock;
        std::map<int32_t, Player> m_players;
        std::map<int, bool> m_availableControllerIndexMap;
    };
}
