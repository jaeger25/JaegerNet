#pragma once

#include <cstddef>
#include <functional>
#include <asio.hpp>
#include "JaegerNet.pb.h"
#include "MessageHandler.h"

namespace JaegerNet
{
    enum { MaxPacketSize = 65535 };

    class IServer
    {
    public:
        virtual void Send(const JaegerNetMessage& message) = 0;
    };

    class Server : IServer
    {
    public:
        Server(asio::io_service& service, short port);
        virtual ~Server();

        virtual void Send(const JaegerNetMessage& message);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        std::array<char, MaxPacketSize> m_data;
    };
}
