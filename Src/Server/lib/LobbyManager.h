#pragma once

#include <shared_mutex>
#include "Lobby.h"
#include "MessageHandler.h"

namespace JaegerNet
{
    enum { MaxLobbies = 512 };

    class IServer;

    class LobbyManager : public IMessageHandler
    {
    public:
        LobbyManager();
        virtual ~LobbyManager();

        // IMessageHandler
        virtual void OnMessageReceived(IServer& sender, MessageReceivedEventArgs& eventArgs) noexcept;

    private:
        std::shared_mutex m_lobbiesLock;
        std::map<int32_t, Lobby> m_lobbies;
    };
}
