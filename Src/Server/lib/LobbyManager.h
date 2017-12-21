#pragma once

#include <map>
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
        virtual void OnMessageReceived(IServer* const sender, MessageReceivedEventArgs& eventArgs) noexcept;

    private:
        JaegerNetResponse&& HandleCreateLobbyRequest(MessageReceivedEventArgs& eventArgs) noexcept;
        JaegerNetResponse&& HandleConnectRequest(MessageReceivedEventArgs& eventArgs) noexcept;

        std::shared_mutex m_lobbiesLock;
        std::map<int32_t, Lobby> m_lobbies;
    };
}
