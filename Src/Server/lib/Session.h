#pragma once
#include "Lobby.h"
#include "Server.h"
#include "JaegerNet.pb.h"

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
        Lobby& Lobby();

    private:
        JaegerNet::Server m_server;
        JaegerNet::Lobby m_lobby;
    };
}
