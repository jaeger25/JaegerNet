#pragma once

#include <cstddef>
#include <functional>
#include <asio.hpp>
#include "JaegerNet.pb.h"
#include "MessageHandler.h"

namespace JaegerNet
{
    enum { MaxPacketSize = 65535 };

    class Server
    {
    public:
        Server(MessageHandler messageHandler, asio::io_service& ioService, short port);

        void Send(std::size_t length);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        MessageHandler m_messageHandler;
        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        std::array<char, MaxPacketSize> m_data;
    };
}
