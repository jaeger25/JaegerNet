#pragma once

#include <asio.hpp>
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class IClient;

    struct MessageReceivedEventArgs
    {
        const JaegerNetResponse& Message;
    };

    class IMessageHandler
    {
    public:
        virtual ~IMessageHandler() = default;

        virtual void OnMessageReceived(IClient* const sender, MessageReceivedEventArgs& eventArgs) noexcept = 0;
    };
}
