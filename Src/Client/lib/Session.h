#pragma once
#include "Client.h"
#include "InputListener.h"
#include "Lobby.h"

namespace JaegerNet
{
    struct SessionDetails
    {
        const std::string Hostname;
        const std::string Port;
    };

    class Session
    {
    public:
        Session(const SessionDetails& client);
        ~Session();

        Client& Client();
        InputListener& InputListener();
        Lobby& Lobby();

    private:
        void OnBroadcastReceived(const BroadcastReceivedEventArgs& args);
        void OnControllerStateChanged(const Controller& controller);

        int32_t m_controllerStateChangedToken;
        int32_t m_broadcastReceivedToken;

        JaegerNet::InputListener m_inputListener;
        JaegerNet::Lobby m_lobby;
        JaegerNet::Client m_client;
    };
}
