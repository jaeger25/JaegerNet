#include "LobbyManager.h"

using namespace JaegerNet;
using namespace std;

LobbyManager::LobbyManager()
{

}

LobbyManager::~LobbyManager()
{

}

void LobbyManager::OnMessageReceived(const IServer& sender, MessageReceivedEventArgs& eventArgs) noexcept
{
    auto message = eventArgs.Message;
    if (message.has_createlobby())
    {
        auto createLobbyMessage = message.createlobby();
    }
    else if (message.has_connect())
    {
        auto connectMessage = message.connect();
    }
}