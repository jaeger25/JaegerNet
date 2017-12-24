#pragma once
#include <memory>

namespace JaegerNet
{
    class Controller;

    class Player
    {
    public:
        Player(int32_t playerId, int controllerIndex);
        ~Player();

        int ControllerIndex() const;

        void OnControllerStateChanged(const Controller& controller);

    private:
        int32_t m_playerId;
        int m_controllerIndex;
    };
}
