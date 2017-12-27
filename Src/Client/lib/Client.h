#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <thread>
#include <asio.hpp>
#include "Constants.h"
#include "ErrorCodes.h"
#include "Event.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class BroadcastReceivedEventArgs
    {
    public:
        const JaegerNetBroadcast& Broadcast;
    };

    typedef std::function<void(const JaegerNetResponse& response)> ResponseReceivedCallback;
    typedef std::function<void(const BroadcastReceivedEventArgs& args)> BroadcastReceivedCallback;

    class IClient
    {
    public:
        virtual void Send(JaegerNetRequest& message, ResponseReceivedCallback&& callback) = 0;
        virtual void Run(bool runAsync) = 0;

        virtual int32_t BroadcastReceived(BroadcastReceivedCallback&& callback) = 0;
        virtual void BroadcastReceived(int32_t token) = 0;
    };

    class Client : public IClient
    {
    public:
        Client(std::string hostName, std::string port);
        virtual ~Client();

        virtual void Send(JaegerNetRequest& message, ResponseReceivedCallback&& callback);
        virtual void Run(bool runAsync);

        virtual int32_t BroadcastReceived(BroadcastReceivedCallback&& callback);
        virtual void BroadcastReceived(int32_t token);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        EventSource<const BroadcastReceivedEventArgs&> m_broadcastReceivedEventSource;

        asio::io_service m_service;
        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        asio::ip::udp::endpoint m_serverEndpoint;
        std::thread m_serviceThread;
        std::array<std::byte, MaxPacketSize> m_receivedData;
        std::array<std::byte, MaxPacketSize> m_sentData;
        std::map<uint64_t, ResponseReceivedCallback> m_responseCallbackMap;
    };

    extern void CreateClient(const char* const hostname, const char* port);
    extern void DestroyClient(void);
    extern std::shared_ptr<IClient> GetClient(void) noexcept;
}
