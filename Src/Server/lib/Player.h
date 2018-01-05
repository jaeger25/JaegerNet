#pragma once
#include <asio.hpp>
#include <map>
#include "JaegerNet_Types.h"
#include "Server.h"

namespace JaegerNet
{
    struct ControllerStateChangedEventArgs
    {
        int PlayerNumber;
        ControllerState State;
    };

    typedef std::function<void(const ControllerStateChangedEventArgs& args)> ControllerStateChangedCallback;

    class Player
    {
    public:
        Player(int playerNumber, Server& server, asio::ip::udp::endpoint&& endpoint) noexcept;
        Player(Player&& other) noexcept;
        Player(const Player& other) = delete;
        Player& operator=(const Player&) = delete;

        ~Player();

        int PlayerNumber() const;
        asio::ip::udp::endpoint& Endpoint();

        EventRegistrationToken ControllerStateChanged(ControllerStateChangedCallback&& callback);
        void ControllerStateChanged(EventRegistrationToken token);

    private:
        void HandleInputRequest(RequestReceivedEventArgs& args);
        void OnRequestReceived(RequestReceivedEventArgs& args);

        EventRegistrationToken m_requestReceivedToken;

        EventSource<ControllerStateChangedEventArgs> m_controllerStateChangedEventSource;
        uint64_t m_nextMessageNumber = 1;
        int m_playerNumber;
        Server& m_server;
        asio::ip::udp::endpoint m_endpoint;
        std::map<uint64_t, ControllerState> m_controllerStates;
    };
}
