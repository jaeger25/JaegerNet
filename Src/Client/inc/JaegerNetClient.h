#pragma once
#include <stdint.h>
#include <functional>
#include "JaegerNetClient_Exports.h"
#include "JaegerNet_Types.h"
#include "JaegerNet_ErrorCodes.h"

typedef std::function<void(JaegerNet::JaegerNetError error)> JaegerNet_ErrorCallback;
typedef std::function<void(int playerNumber)> JaegerNet_PlayerConnectedCallback;
typedef std::function<void(int playerNumber)> JaegerNet_PlayerDisconnectedCallback;
typedef std::function<void(int controllerIndex)> JaegerNet_ControllerAddedCallback;
typedef std::function<void(int controllerIndex)> JaegerNet_ControllerRemovedCallback;

void JAEGERNET_EXPORT JaegerNet_StartClient(const char* const hostname, const char* const port);
void JAEGERNET_EXPORT JaegerNet_StopClient(void);

JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterControllerAddedCallback(JaegerNet_ControllerAddedCallback callback);
JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterControllerRemovedCallback(JaegerNet_ControllerRemovedCallback callback);
void JAEGERNET_EXPORT JaegerNet_UnregisterControllerAddedCallback(JaegerNet::EventRegistrationToken token);
void JAEGERNET_EXPORT JaegerNet_UnregisterControllerRemovedCallback(JaegerNet::EventRegistrationToken token);

JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterPlayerConnectedCallback(JaegerNet_PlayerConnectedCallback callback);
JaegerNet::EventRegistrationToken JAEGERNET_EXPORT JaegerNet_RegisterPlayerDisconnectedCallback(JaegerNet_PlayerDisconnectedCallback callback);
void JAEGERNET_EXPORT JaegerNet_UnregisterPlayerConnectedCallback(JaegerNet::EventRegistrationToken token);
void JAEGERNET_EXPORT JaegerNet_UnregisterPlayerDisconnectedCallback(JaegerNet::EventRegistrationToken token);

void JAEGERNET_EXPORT JaegerNet_Connect(int controllerIndex, JaegerNet_ErrorCallback errorCallback);
