#include "Player.h"
#include "Client.h"
#include "Controller.h"
#include "JaegerNet.pb.h"
#include "ControllerInputMessage.pb.h"

using namespace JaegerNet;

Player::Player(int32_t playerId) :
    m_playerId(playerId)
{
}

Player::Player()
{
}

Player::~Player()
{
}

int32_t Player::PlayerId() const
{
    return m_playerId;
}

void Player::PlayerId(int32_t playerId)
{
    m_playerId = playerId;
}

int32_t Player::PlayerNumber() const
{
    return m_playerNumber;
}

void Player::PlayerNumber(int32_t playerNumber)
{
    m_playerNumber = playerNumber;
}
