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

    class Client
    {
    public:
        Client(std::string hostName, std::string port);
        ~Client();

        void Send(JaegerNetRequest& message, ResponseReceivedCallback&& callback);
        void Run(bool runAsync);

        int32_t BroadcastReceived(BroadcastReceivedCallback&& callback);
        void BroadcastReceived(int32_t token);

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
}
