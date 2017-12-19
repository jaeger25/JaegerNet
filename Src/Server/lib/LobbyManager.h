#pragma once

#include "Lobby.h"
#include "MessageHandler.h"

namespace JaegerNet
{
    class IServer;

    class LobbyManager : IMessageHandler
    {
    public:
        LobbyManager();
        virtual ~LobbyManager();

        // IMessageHandler
        virtual void OnMessageReceived(const IServer& sender, MessageReceivedEventArgs& eventArgs) noexcept;

    private:
        std::map<int32_t, Lobby> m_lobbies;
    };
}
