#pragma once

#include <asio.hpp>
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class Server
    {
    public:
        Server(asio::io_service& ioService, short port);

        void do_receive();

        void Send(std::size_t length);

    private:
        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        std::array<char, 508> m_data;
    };
}
