#pragma once
#include <asio.hpp>
#include "Server.h"

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int playerNumber, asio::ip::udp::endpoint&& endpoint);
        virtual ~Player();

        int PlayerNumber() const;
        asio::ip::udp::endpoint& Endpoint();

    private:
        int m_playerNumber;
        asio::ip::udp::endpoint m_endpoint;
    };
}
