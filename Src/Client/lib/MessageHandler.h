#pragma once

#include <asio.hpp>

namespace JaegerNet
{
    class IClient;
    class JaegerNetResponse;

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
