#include "Session.h"
#include "Server.h"

using namespace JaegerNet;

Session::Session(const SessionDetails& details) :
    m_server(details.Port),
    m_lobby(m_server)
{
}

Session::~Session()
{
}

Server& Session::Server()
{
    return m_server;
}

Lobby& Session::Lobby()
{
    return m_lobby;
}
