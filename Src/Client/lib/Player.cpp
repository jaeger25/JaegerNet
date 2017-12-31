#include "Player.h"
#include "Client.h"
#include "Controller.h"
#include "JaegerNet.pb.h"
#include "ControllerInputMessage.pb.h"

using namespace JaegerNet;

Player::Player(int playerNumber, Client& client, InputListener& inputListener) noexcept :
    m_playerNumber(playerNumber),
    m_client(client),
    m_inputListener(inputListener)
{
    m_controllerStateChangedToken = m_inputListener.ControllerStateChanged([this](const Controller& controller)
    {
        OnControllerStateChanged(controller);
    });
}

Player::~Player()
{
    m_inputListener.ControllerStateChanged(m_controllerStateChangedToken);
}

int Player::PlayerNumber() const
{
    return m_playerNumber;
}

void Player::PlayerNumber(int playerNumber)
{
    m_playerNumber = playerNumber;
}

void Player::OnControllerStateChanged(const Controller& controller)
{
    auto controllerState = controller.CurrentState();

    auto inputRequest = std::make_unique<ControllerInputRequest>();
    auto controllerInput = inputRequest->add_controllerinput();

    controllerInput->set_axisvalue(controllerState.AxisValue);
    controllerInput->set_controllerbuttonstate(static_cast<int32_t>(controllerState.ButtonState));
    controllerInput->set_controllerdpadbuttonstate(static_cast<int32_t>(controllerState.DPadButtonState));

    inputRequest->set_playernumber(m_playerNumber);

    JaegerNetRequest request;
    request.set_allocated_controllerinputrequest(inputRequest.release());

    m_client.Send(request, [](const JaegerNetResponse& /*response*/)
    {
    });
}
