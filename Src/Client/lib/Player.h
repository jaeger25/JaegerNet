#pragma once
#include <memory>

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int32_t playerId);
        ~Player();

    private:
        int32_t m_playerId;
    };
}
