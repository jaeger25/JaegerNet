#pragma once
#include <asio.hpp>
#include <queue>
#include "JaegerNet_Types.h"
#include "Server.h"

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int playerNumber, Server& server, asio::ip::udp::endpoint&& endpoint);
        virtual ~Player();

        int PlayerNumber() const;
        asio::ip::udp::endpoint& Endpoint();

        void PushControllerState(const ControllerState& state);
        ControllerState PopControllerState();

    private:
        void HandleInputRequest(RequestReceivedEventArgs& args);
        void OnRequestReceived(RequestReceivedEventArgs& args);

        EventRegistrationToken m_requestReceivedToken;

        int m_playerNumber;
        Server& m_server;
        asio::ip::udp::endpoint m_endpoint;
        std::queue<ControllerState> m_controllerStates;
    };
}
