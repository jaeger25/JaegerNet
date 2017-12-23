#include "Player.h"

using namespace JaegerNet;
using namespace std;

Player::Player(int32_t playerId, int playerNumber) :
    m_playerId(playerId),
    m_playerNumber(playerNumber)
{
}

Player::~Player()
{
}

int32_t Player::PlayerId() const
{
    return m_playerId;
}

int Player::PlayerNumber() const
{
    return m_playerNumber;
}
