#include "Player.h"

using namespace JaegerNet;
using namespace std;

Player::Player(int playerNumber) :
    m_playerNumber(playerNumber)
{

}

Player::~Player()
{

}

int Player::PlayerNumber() const
{
    return m_playerNumber;
}
