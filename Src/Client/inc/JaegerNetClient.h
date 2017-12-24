#pragma once
#include <stdint.h>
#include "JaegerNetClient_Exports.h"
#include "ErrorCodes.h"

typedef void(*CreateLobbyCallback)(int32_t lobbyId, JaegerNet::JaegerNetError error);
typedef void(*ConnectCallback)(int32_t playerId, int32_t playerNumber, JaegerNet::JaegerNetError error);
typedef void(*DisconnectCallback)(JaegerNet::JaegerNetError error);
typedef void(*ControllerAddedCallback)(int controllerIndex);
typedef void(*ControllerRemovedCallback)(int controllerIndex);

void JAEGERNET_EXPORT JaegerNet_StartClient(const char* const hostname, const char* const port);
void JAEGERNET_EXPORT JaegerNet_StopClient(void);

void JAEGERNET_EXPORT JaegerNet_StartInputListener(::ControllerAddedCallback controllerAddedCallback, ::ControllerRemovedCallback controllerRemovedCallback);
void JAEGERNET_EXPORT JaegerNet_StopInputListener(void);

void JAEGERNET_EXPORT JaegerNet_CreateLobby(CreateLobbyCallback callback);
void JAEGERNET_EXPORT JaegerNet_Connect(int32_t lobbyId, ConnectCallback callback);
void JAEGERNET_EXPORT JaegerNet_Disconnect(int32_t playerId, DisconnectCallback callback);
