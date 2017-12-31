#include "Player.h"
#include "ControllerInputMessage.pb.h"

using namespace asio::ip;
using namespace JaegerNet;
using namespace std;

Player::Player(int playerNumber, Server& server, udp::endpoint&& endpoint) :
    m_playerNumber(playerNumber),
    m_server(server),
    m_endpoint(std::move(endpoint))
{
    m_requestReceivedToken = m_server.RequestReceived([this](RequestReceivedEventArgs& args)
    {
        OnRequestReceived(args);
    });
}

Player::~Player()
{
    m_server.RequestReceived(m_requestReceivedToken);
}

int Player::PlayerNumber() const
{
    return m_playerNumber;
}

udp::endpoint& Player::Endpoint()
{
    return m_endpoint;
}

void Player::PushControllerState(const ControllerState& state)
{
    m_controllerStates.push(state);
}

ControllerState Player::PopControllerState()
{
    auto state = m_controllerStates.front();
    m_controllerStates.pop();

    return state;
}

void Player::HandleInputRequest(RequestReceivedEventArgs& /*args*/)
{

}

void Player::OnRequestReceived(RequestReceivedEventArgs& args)
{
    if (args.Request.has_controllerinputrequest() && args.Request.controllerinputrequest().playernumber() == m_playerNumber)
    {
        HandleInputRequest(args);
    }
}
