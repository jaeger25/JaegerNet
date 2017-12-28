#include "Player.h"

using namespace asio::ip;
using namespace JaegerNet;
using namespace std;

Player::Player(int32_t playerId, int playerNumber, asio::ip::udp::endpoint&& endpoint) :
    m_playerId(playerId),
    m_playerNumber(playerNumber),
    m_endpoint(std::move(endpoint))
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

void Player::Send(IServer* const server, JaegerNetBroadcast& message)
{
    server->Send(m_endpoint, message);
}
