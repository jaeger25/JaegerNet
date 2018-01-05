#include "Player.h"
#include "ControllerInputMessage.pb.h"

using namespace asio::ip;
using namespace JaegerNet;
using namespace std;

Player::Player(int playerNumber, Server& server, udp::endpoint&& endpoint) noexcept :
    m_playerNumber(playerNumber),
    m_server(server),
    m_endpoint(std::move(endpoint))
{
    m_requestReceivedToken = m_server.RequestReceived([this](RequestReceivedEventArgs& args)
    {
        OnRequestReceived(args);
    });
}

Player::Player(Player&& other) noexcept :
    m_playerNumber(other.m_playerNumber),
    m_server(other.m_server),
    m_endpoint(std::move(other.m_endpoint)),
    m_controllerStateChangedEventSource(std::move(other.m_controllerStateChangedEventSource)),
    m_controllerStates(std::move(other.m_controllerStates)),
    m_nextMessageNumber(other.m_nextMessageNumber),
    m_requestReceivedToken(other.m_requestReceivedToken)
{
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

EventRegistrationToken Player::ControllerStateChanged(ControllerStateChangedCallback&& callback)
{
    return m_controllerStateChangedEventSource.Add(std::move(callback));
}

void Player::ControllerStateChanged(EventRegistrationToken token)
{
    m_controllerStateChangedEventSource.Remove(token);
}

void Player::HandleInputRequest(RequestReceivedEventArgs& args)
{
    auto inputRequest = args.Request.controllerinputrequest();

    ControllerState controllerState;
    for (auto&& state : inputRequest.controllerinput())
    {
        if (!m_controllerStates.count(state.messagenumber()))
        {
            controllerState.AxisValue = static_cast<int16_t>(state.axisvalue());
            controllerState.ButtonState = static_cast<ControllerButton>(state.controllerbuttonstate());
            controllerState.DPadButtonState = static_cast<ControllerDPadButton>(state.controllerdpadbuttonstate());

            m_controllerStates.emplace(state.messagenumber(), std::move(controllerState));
        }
    }

    for (auto iter = m_controllerStates.begin(); iter != m_controllerStates.end();)
    {
        if (iter->first == m_nextMessageNumber)
        {
            ControllerStateChangedEventArgs stateChangedArgs{ m_playerNumber, iter->second };
            m_controllerStateChangedEventSource.Invoke(stateChangedArgs);
            iter = m_controllerStates.erase(iter);
            m_nextMessageNumber++;
        }
        else
        {
            break;
        }
    }

    auto inputResponse = std::make_unique<ControllerInputResponse>();
    inputResponse->set_playernumber(inputRequest.playernumber());
    inputResponse->set_highestmessagenumber(m_nextMessageNumber - 1);

    args.Response.set_allocated_controllerinputresponse(inputResponse.release());
}

void Player::OnRequestReceived(RequestReceivedEventArgs& args)
{
    if (args.Request.has_controllerinputrequest() && args.Request.controllerinputrequest().playernumber() == m_playerNumber)
    {
        HandleInputRequest(args);
    }
}
