#include "JaegerNetClient.h"
#include "Session.h"
#include "Logging.h"

using namespace JaegerNet;

std::unique_ptr<Session> JaegerNetClientSession;
std::shared_ptr<spdlog::logger> Logger;

void JaegerNet_StartClient(const char* const hostname, const char* const port)
{
    Logger = spdlog::stdout_color_mt("console");
    Logger->set_level(spdlog::level::trace);

    SessionDetails details{ std::string(hostname), std::string(port) };

    JaegerNetClientSession = std::make_unique<Session>(details);
    JaegerNetClientSession->Client().Run(true);
}

void JaegerNet_StopClient(void)
{
    JaegerNetClientSession.reset();

    Logger.reset();
}

void JaegerNet_Connect(int controllerIndex, JaegerNet_ErrorCallback errorCallback)
{
    auto connectRequest = std::make_unique<ConnectRequest>();

    JaegerNetRequest request;
    request.set_allocated_connectrequest(connectRequest.release());

    JaegerNetClientSession->Client().Send(request, [controllerIndex, errorCallback](const JaegerNetResponse& response)
    {
        auto error = static_cast<JaegerNetError>(response.error());
        errorCallback(error);
        if (error == JaegerNetError::Success)
        {
            JaegerNetClientSession->Lobby().BindPlayerToController(response.connectresponse().playernumber(), controllerIndex);
        }
    });
}

EventRegistrationToken JaegerNet_RegisterControllerAddedCallback(JaegerNet_ControllerAddedCallback callback)
{
    return JaegerNetClientSession->InputListener().ControllerAdded(std::move(callback));
}

EventRegistrationToken JaegerNet_RegisterControllerRemovedCallback(JaegerNet_ControllerRemovedCallback callback)
{
    return JaegerNetClientSession->InputListener().ControllerRemoved(std::move(callback));
}

void JaegerNet_UnregisterControllerAddedCallback(EventRegistrationToken token)
{
    JaegerNetClientSession->InputListener().ControllerAdded(token);
}

void JaegerNet_UnregisterControllerRemovedCallback(EventRegistrationToken token)
{
    JaegerNetClientSession->InputListener().ControllerRemoved(token);
}

EventRegistrationToken JaegerNet_RegisterPlayerConnectedCallback(JaegerNet_PlayerConnectedCallback callback)
{
    return JaegerNetClientSession->Lobby().PlayerConnected(std::move(callback));
}

EventRegistrationToken JaegerNet_RegisterPlayerDisconnectedCallback(JaegerNet_PlayerDisconnectedCallback callback)
{
    return JaegerNetClientSession->Lobby().PlayerDisconnected(std::move(callback));
}

void JaegerNet_UnregisterPlayerConnectedCallback(EventRegistrationToken token)
{
    JaegerNetClientSession->Lobby().PlayerConnected(token);
}

void JaegerNet_UnregisterPlayerDisconnectedCallback(EventRegistrationToken token)
{
    JaegerNetClientSession->Lobby().PlayerDisconnected(token);
}
