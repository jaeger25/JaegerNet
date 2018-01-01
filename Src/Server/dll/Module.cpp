#include "JaegerNetServer.h"
#include "ErrorCodes.h"
#include "Session.h"

using namespace JaegerNet;

std::unique_ptr<Session> JaegerNetServerSession;

void JaegerNet_StartServer(short port)
{
    SessionDetails details{ port };
    JaegerNetServerSession = std::make_unique<Session>(details);
}

void JaegerNet_StopServer(void)
{
    JaegerNetServerSession.reset();
}

EventRegistrationToken JaegerNet_RegisterPlayerConnectedCallback(JaegerNet_PlayerConnectedCallback callback)
{
    return JaegerNetServerSession->Lobby().PlayerConnected([callback](const PlayerConnectedEventArgs& args)
    {
        callback(args.Player->PlayerNumber());
    });
}

EventRegistrationToken JaegerNet_RegisterPlayerDisconnectedCallback(JaegerNet_PlayerDisconnectedCallback /*callback*/)
{
    return 0;
}

EventRegistrationToken JaegerNet_RegisterControllerStateChangedCallback(int playerNumber, JaegerNet_ControllerStateChangedCallback callback)
{
    for (auto&& player : JaegerNetServerSession->Lobby().Players())
    {
        if (player->PlayerNumber() == playerNumber)
        {
            return player->ControllerStateChanged([callback](const ControllerStateChangedEventArgs& args)
            {
                callback(args.PlayerNumber, args.State);
            });

            break;
        }
    }

    throw JaegerNetException(JaegerNetError::PlayerNotFound);
}

void JaegerNet_UnregisterPlayerConnectedCallback(EventRegistrationToken token)
{
    JaegerNetServerSession->Lobby().PlayerConnected(token);
}

void JaegerNet_UnregisterPlayerDisconnectedCallback(EventRegistrationToken /*token*/)
{

}

void JaegerNet_UnregisterControllerStateChangedCallback(int playerNumber, EventRegistrationToken token)
{
    for (auto&& player : JaegerNetServerSession->Lobby().Players())
    {
        if (player->PlayerNumber() == playerNumber)
        {
            player->ControllerStateChanged(token);
            break;
        }
    }
}
