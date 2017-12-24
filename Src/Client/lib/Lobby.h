#pragma once

#include <map>
#include <queue>
#include <shared_mutex>
#include "Player.h"

namespace JaegerNet
{
    class Controller;

    class Lobby
    {
    public:
        static Lobby& Instance();

        ~Lobby();
        void AddPlayer(int32_t playerId);
        void RemovePlayer(int32_t playerId);

    private:
        Lobby();
        void OnControllerAdded(int controllerIndex);
        void OnControllerStateChanged(const Controller& controller);

        int32_t m_controllerAddedToken;
        int32_t m_controllerStateChangedToken;

        std::shared_mutex m_playersLock;
        std::map<int32_t, Player> m_players;
        std::queue<int> m_availableControllerIndices;
    };
}
