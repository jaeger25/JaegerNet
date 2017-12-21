#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <thread>
#include <asio.hpp>
#include "Constants.h"
#include "ErrorCodes.h"
#include "MessageHandler.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    typedef std::function<void(const JaegerNetResponse& response)> ResponseReceivedCallback;

    class IClient
    {
    public:
        virtual void Send(JaegerNetRequest& message, ResponseReceivedCallback&& callback) = 0;
        virtual void Run(bool runAsync) = 0;
    };

    class Client : public IClient
    {
    public:
        Client(std::string hostName, std::string port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers);
        virtual ~Client();

        // IClient
        virtual void Send(JaegerNetRequest& message, ResponseReceivedCallback&& callback);
        virtual void Run(bool runAsync);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        asio::io_service m_service;
        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        asio::ip::udp::endpoint m_serverEndpoint;
        std::thread m_serviceThread;
        std::array<std::byte, MaxPacketSize> m_receivedData;
        std::array<std::byte, MaxPacketSize> m_sentData;
        std::vector<std::unique_ptr<IMessageHandler>> m_messageHandlers;
        std::map<uint64_t, ResponseReceivedCallback> m_responseCallbackMap;
    };

    extern void CreateClient(const char* const hostname, const char* port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers);
    extern void DestroyClient(void);
    extern std::shared_ptr<IClient> GetClient(void) noexcept;
}
