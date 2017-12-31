#pragma once
#include "LobbyManager.h"
#include "Server.h"

namespace JaegerNet
{
    struct SessionDetails
    {
        short Port;
    };

    class Session
    {
    public:
        Session(const SessionDetails& details);
        ~Session();

        Server& Server();
        LobbyManager& LobbyManager();

    private:
        //void OnRequestReceived(const BroadcastReceivedEventArgs& args);

        int32_t m_broadcastReceivedToken;

        JaegerNet::Server m_server;
        JaegerNet::LobbyManager m_lobbyManager;
    };
}
