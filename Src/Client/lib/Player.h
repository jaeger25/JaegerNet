#pragma once
#include <map>
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
        void OnControllerInputResponse(const ControllerInputResponse& response);

        int m_playerNumber;
        uint64_t m_nextMessageNumber = 1;
        Client& m_client;
        InputListener& m_inputListener;
        EventRegistrationToken m_controllerStateChangedToken;

        std::mutex m_controllerStateLock;
        std::map<uint64_t, ControllerState> m_pendingControllerStates;
    };
}
