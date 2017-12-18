#pragma once

#include <asio.hpp>
#include "JaegerNet.pb.h"

namespace JaegerNet
{
    class MessageHandler
    {
    public:
        void OnMessageReceived(const JaegerNetMessage& message);

    private:
    };
}
