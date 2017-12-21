#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <asio.hpp>
#include "Constants.h"
#include "MessageHandler.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class IClient
    {
    public:
        virtual void Send(const JaegerNetRequest& message) = 0;
    };

    class Client : public IClient
    {
    public:
        Client(asio::io_service& service, std::string hostName, std::string port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers);
        virtual ~Client();

        virtual void Send(const JaegerNetRequest& message);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        asio::ip::udp::endpoint m_serverEndpoint;
        std::array<std::byte, MaxPacketSize> m_receivedData;
        std::array<std::byte, MaxPacketSize> m_sentData;
        std::vector<std::unique_ptr<IMessageHandler>> m_messageHandlers;
    };
}
