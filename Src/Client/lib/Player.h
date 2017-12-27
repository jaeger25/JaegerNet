#pragma once
#include <memory>

namespace JaegerNet
{
    class Controller;

    class Player
    {
    public:
        Player(int32_t playerId, int controllerIndex);
        Player();
        ~Player();

        int32_t PlayerId() const;
        void PlayerId(int32_t playerId);

        int32_t PlayerNumber() const;
        void PlayerNumber(int32_t playerNumber);

        int ControllerIndex() const;

        void OnControllerStateChanged(const Controller& controller);

    private:
        int32_t m_playerId;
        int32_t m_playerNumber;
        int m_controllerIndex;
    };
}
