#include "Player.h"
#include "Client.h"
#include "Controller.h"
#include "JaegerNet.pb.h"
#include "ControllerInputMessage.pb.h"

using namespace JaegerNet;

Player::Player(int playerNumber) :
    m_playerNumber(playerNumber)
{
}

Player::Player()
{
}

Player::~Player()
{
}

int Player::PlayerNumber() const
{
    return m_playerNumber;
}

void Player::PlayerNumber(int playerNumber)
{
    m_playerNumber = playerNumber;
}
