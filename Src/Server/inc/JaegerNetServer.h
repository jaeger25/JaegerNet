#pragma once
#include <functional>
#include "JaegerNetServer_Exports.h"
#include "JaegerNet_Types.h"

typedef std::function<void(int playerNumber)> JaegerNet_PlayerConnectedCallback;
typedef std::function<void(int playerNumber)> JaegerNet_PlayerDisconnectedCallback;
typedef std::function<void(int playerNumber, const JaegerNet::ControllerState state)> JaegerNet_ControllerStateChangedCallback;

void JAEGERNET_EXPORT JaegerNet_StartServer(short port);
void JAEGERNET_EXPORT JaegerNet_StopServer(void);

JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterPlayerConnectedCallback(JaegerNet_PlayerConnectedCallback callback);
JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterPlayerDisconnectedCallback(JaegerNet_PlayerDisconnectedCallback callback);
JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterControllerStateChangedCallback(int playerNumber, JaegerNet_ControllerStateChangedCallback callback);
void JAEGERNET_EXPORT JaegerNet_UnregisterPlayerConnectedCallback(JaegerNet::EventRegistrationToken token);
void JAEGERNET_EXPORT JaegerNet_UnregisterPlayerDisconnectedCallback(JaegerNet::EventRegistrationToken token);
void JAEGERNET_EXPORT JaegerNet_UnregisterControllerStateChangedCallback(int playerNumber, JaegerNet::EventRegistrationToken token);
