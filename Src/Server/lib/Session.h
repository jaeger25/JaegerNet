#pragma once
#include "Lobby.h"
#include "NoCopy.h"
#include "Server.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    struct SessionDetails
    {
        short Port;
    };

    class Session : NoCopy
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
