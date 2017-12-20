#pragma once

#include <asio.hpp>
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class IServer;

    struct MessageReceivedEventArgs
    {
        const JaegerNetMessage& Message;
    };

    class IMessageHandler
    {
    public:
        virtual ~IMessageHandler() = default;

        virtual void OnMessageReceived(IServer& sender, MessageReceivedEventArgs& eventArgs) noexcept = 0;
    };
}
