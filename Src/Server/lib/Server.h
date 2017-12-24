#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <asio.hpp>
#include "Constants.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class IMessageHandler;

    class IServer
    {
    public:
        virtual void Send(const JaegerNetResponse& message) = 0;
        virtual void Send(asio::ip::udp::endpoint& endpoint, const JaegerNetBroadcast& message) = 0;
    };

    class Server : public IServer
    {
    public:
        Server(asio::io_service& service, short port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers);
        virtual ~Server();

        // IServer
        virtual void Send(const JaegerNetResponse& message);
        virtual void Send(asio::ip::udp::endpoint& endpoint, const JaegerNetBroadcast& message);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        std::array<std::byte, MaxPacketSize> m_receievedData;
        std::array<std::byte, MaxPacketSize> m_sentData;
        std::vector<std::unique_ptr<IMessageHandler>> m_messageHandlers;
    };
}
