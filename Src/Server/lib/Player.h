#pragma once

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int playerNumber);
        virtual ~Player();

        int PlayerNumber() const;

    private:
        int m_playerNumber;
    };
}
