#pragma once

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int32_t playerId);
        Player();
        ~Player();

        int32_t PlayerId() const;
        void PlayerId(int32_t playerId);

        int32_t PlayerNumber() const;
        void PlayerNumber(int32_t playerNumber);

    private:
        int32_t m_playerId;
        int32_t m_playerNumber;
    };
}
