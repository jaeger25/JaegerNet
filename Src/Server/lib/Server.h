#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <asio.hpp>
#include "Constants.h"
#include "Event.h"
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    struct RequestReceivedEventArgs
    {
        asio::ip::udp::endpoint Endpoint;
        const JaegerNetRequest& Request;
    };

    typedef std::function<void(const RequestReceivedEventArgs& args)> RequestReceivedCallback;

    class Server
    {
    public:
        Server(short port);
        virtual ~Server();

        void Send(const JaegerNetResponse& message);
        void Send(asio::ip::udp::endpoint& endpoint, JaegerNetBroadcast& message);
        void Run(bool runAsync);

        int32_t RequestReceived(RequestReceivedCallback&& callback);
        void RequestReceived(int32_t token);

    private:
        void StartReceive();
        void OnDataReceived(const std::error_code& error, std::size_t /*bytesReceived*/);
        void OnDataSent(const std::error_code& error, std::size_t /*bytesReceived*/);

        EventSource<const RequestReceivedEventArgs&> m_requestReceivedEventSource;

        asio::io_service m_service;
        asio::ip::udp::socket m_socket;
        asio::ip::udp::endpoint m_endpoint;
        std::array<std::byte, MaxPacketSize> m_receievedData;
        std::array<std::byte, MaxPacketSize> m_sentData;
        std::thread m_serviceThread;
    };
}
