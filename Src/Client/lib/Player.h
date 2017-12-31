#pragma once
#include "Client.h"
#include "Event.h"
#include "InputListener.h"
#include "NoCopy.h"

namespace JaegerNet
{
    class Player : NoCopy
    {
    public:
        Player(int playerNumber, Client& client, InputListener& inputListener) noexcept;
        ~Player();

        int PlayerNumber() const;
        void PlayerNumber(int playerNumber);

    private:
        void OnControllerStateChanged(const Controller& controller);

        int m_playerNumber;
        Client& m_client;
        InputListener& m_inputListener;
        EventRegistrationToken m_controllerStateChangedToken;
    };
}
