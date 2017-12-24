#pragma once
#include <asio.hpp>
#include "Server.h"

namespace JaegerNet
{
    class Player
    {
    public:
        Player(int32_t playerId, int playerNumber, asio::ip::udp::endpoint&& endpoint);
        virtual ~Player();

        int32_t PlayerId() const;
        int PlayerNumber() const;

        void Send(IServer* const server, const JaegerNetBroadcast& message);

    private:
        int32_t m_playerId;
        int m_playerNumber;
        asio::ip::udp::endpoint m_endpoint;
    };
}
