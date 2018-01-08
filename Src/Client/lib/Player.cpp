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
    m_controllerStateChangedToken = m_inputListener.ControllerStateChanged(std::bind(&Player::OnControllerStateChanged, this, std::placeholders::_1, std::placeholders::_2));
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

int Player::ControllerIndex() const
{
    return m_controllerIndex;
}

void Player::ControllerIndex(int controllerIndex)
{
    m_controllerIndex = controllerIndex;
}

void Player::OnControllerStateChanged(int controllerIndex, const ControllerState& controllerState)
{
    if (controllerIndex == m_controllerIndex)
    {
        std::lock_guard<std::mutex> lock(m_controllerStateLock);

        m_pendingControllerStates.emplace(m_nextMessageNumber++, std::move(controllerState));

        auto inputRequest = std::make_unique<ControllerInputRequest>();
        for (auto&&[messageNumber, pendingState] : m_pendingControllerStates)
        {
            auto controllerInput = inputRequest->add_controllerinput();

            controllerInput->set_messagenumber(messageNumber);
            controllerInput->set_axisxvalue(pendingState.AxisXValue);
            controllerInput->set_axisyvalue(pendingState.AxisYValue);
            controllerInput->set_controllerbuttonstate(static_cast<int32_t>(pendingState.ButtonState));
            controllerInput->set_controllerdpadbuttonstate(static_cast<int32_t>(pendingState.DPadButtonState));
        }

        inputRequest->set_playernumber(m_playerNumber);

        JaegerNetRequest request;
        request.set_allocated_controllerinputrequest(inputRequest.release());

        m_client.Send(request, [this](const JaegerNetResponse& response)
        {
            OnControllerInputResponse(response.controllerinputresponse());
        });
    }
}

void Player::OnControllerInputResponse(const ControllerInputResponse& response)
{
    std::lock_guard<std::mutex> lock(m_controllerStateLock);

    auto highestMessageIter = m_pendingControllerStates.find(response.highestmessagenumber());
    if (highestMessageIter != m_pendingControllerStates.end())
    {
        // TODO: log
        m_pendingControllerStates.erase(m_pendingControllerStates.begin(), std::next(highestMessageIter));
    }
    else
    {
        //TODO: log
    }
}
