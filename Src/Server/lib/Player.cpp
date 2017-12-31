#include "Player.h"

using namespace asio::ip;
using namespace JaegerNet;
using namespace std;

Player::Player(int playerNumber, asio::ip::udp::endpoint&& endpoint) :
    m_playerNumber(playerNumber),
    m_endpoint(std::move(endpoint))
{
}

Player::~Player()
{
}

int Player::PlayerNumber() const
{
    return m_playerNumber;
}

asio::ip::udp::endpoint& Player::Endpoint()
{
    return m_endpoint;
}
