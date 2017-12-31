#pragma once

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int playerNumber);
        Player();
        ~Player();

        int PlayerNumber() const;
        void PlayerNumber(int playerNumber);

    private:
        int m_playerNumber;
    };
}
