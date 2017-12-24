#pragma once

#include <asio.hpp>
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class IServer;

    struct MessageReceivedEventArgs
    {
        asio::ip::udp::endpoint Endpoint;
        const JaegerNetRequest& Message;
    };

    class IMessageHandler
    {
    public:
        virtual ~IMessageHandler() = default;

        virtual void OnMessageReceived(IServer* const sender, MessageReceivedEventArgs& eventArgs) noexcept = 0;
    };
}
