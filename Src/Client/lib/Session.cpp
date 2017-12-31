#include "Session.h"

using namespace JaegerNet;

Session::Session(const SessionDetails& details) :
    m_client(details.Hostname, details.Port),
    m_lobby(m_client, m_inputListener)
{
    m_broadcastReceivedToken = m_client.BroadcastReceived([this](const BroadcastReceivedEventArgs& args)
    {
        OnBroadcastReceived(args);
    });
}

Session::~Session()
{
    m_client.BroadcastReceived(m_broadcastReceivedToken);
}

Client& Session::Client()
{
    return m_client;
}

InputListener& Session::InputListener()
{
    return m_inputListener;
}

Lobby& Session::Lobby()
{
    return m_lobby;
}

void Session::OnBroadcastReceived(const BroadcastReceivedEventArgs& args)
{
    m_lobby.OnBroadcastReceived(args);
}
