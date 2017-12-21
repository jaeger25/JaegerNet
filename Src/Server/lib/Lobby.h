#pragma once

#include <shared_mutex>
#include "ErrorCodes.h"
#include "Player.h"

namespace JaegerNet
{
    enum { MaxPlayersPerLobby = 4 };

    class Lobby
    {
    public:
        Lobby(int id);
        virtual ~Lobby();

        int Id() const;
        const Player& AddPlayer();

    private:
        int m_id;

        std::shared_mutex m_playersLock;
        std::vector<Player> m_players;
    };
}
