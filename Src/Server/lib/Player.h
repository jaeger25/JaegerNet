#pragma once

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int32_t playerId, int playerNumber);
        virtual ~Player();

        int32_t PlayerId() const;
        int PlayerNumber() const;

    private:
        int32_t m_playerId;
        int m_playerNumber;
    };
}
