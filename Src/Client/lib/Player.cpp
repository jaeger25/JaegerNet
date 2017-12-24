#include "Player.h"
#include "Client.h"
#include "Controller.h"
#include "JaegerNet.pb.h"
#include "ControllerInputMessage.pb.h"

using namespace JaegerNet;

Player::Player(int32_t playerId, int controllerIndex) :
    m_playerId(playerId),
    m_controllerIndex(controllerIndex)
{
}

Player::~Player()
{
}

int Player::ControllerIndex() const
{
    return m_controllerIndex;
}

void Player::OnControllerStateChanged(const Controller& controller)
{
    auto controllerInput = std::make_unique<ControllerInput>();

    controllerInput->set_axisvalue(controller.AxisValue());
    controllerInput->set_controllerbuttonstate(static_cast<int32_t>(controller.ControllerButtonState()));
    controllerInput->set_controllerdpadbuttonstate(static_cast<int32_t>(controller.ControllerDPadButtonState()));
    controllerInput->set_playerid(m_playerId);

    auto inputRequest = std::make_unique<ControllerInputRequest>();
    inputRequest->set_allocated_controllerinput(controllerInput.release());

    JaegerNetRequest request;
    request.set_allocated_controllerinputrequest(inputRequest.release());

    GetClient()->Send(request, [](const JaegerNetResponse& /*response*/)
    {
    });
}
