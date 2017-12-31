#include "Session.h"

using namespace JaegerNet;

Session::Session(const SessionDetails& details) :
    m_server(details.Port),
    m_lobbyManager()
{
    m_broadcastReceivedToken = m_server.BroadcastReceived([this](const BroadcastReceivedEventArgs& args)
    {
        OnBroadcastReceived(args);
    });
}

Session::~Session()
{
    m_server.BroadcastReceived(m_broadcastReceivedToken);
}

Server& Session::Server()
{
    return m_server;
}

LobbyManager& Session::LobbyManager()
{
    return m_lobbyManager;
}

//void Session::OnBroadcastReceived(const BroadcastReceivedEventArgs& args)
//{
//    m_lobby.OnBroadcastReceived(args);
//}
