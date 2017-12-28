#include "Session.h"

using namespace JaegerNet;

Session::Session(const SessionDetails& details) :
    m_client(details.Hostname, details.Port)
{
    m_broadcastReceivedToken = m_client.BroadcastReceived([this](const BroadcastReceivedEventArgs& args)
    {
        OnBroadcastReceived(args);
    });

    m_controllerStateChangedToken = m_inputListener.ControllerStateChanged([this](const Controller& controller)
    {
        OnControllerStateChanged(controller);
    });
}

Session::~Session()
{
    m_inputListener.ControllerStateChanged(m_controllerStateChangedToken);
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

void Session::OnControllerStateChanged(const Controller& controller)
{
    m_lobby.OnControllerStateChanged(controller);
}
