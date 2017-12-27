#pragma once
#include <stdint.h>
#include "JaegerNetClient_Exports.h"
#include "ErrorCodes.h"

typedef void(*JaegerNet_ErrorCallback)(JaegerNet::JaegerNetError error);
typedef void(*JaegerNet_LobbyCreatedCallback)(int32_t lobbyId);
typedef void(*JaegerNet_PlayerConnectedCallback)(int32_t playerNumber);
typedef void(*JaegerNet_DisconnectedCallback)(int32_t playerNumber);
typedef void(*JaegerNet_ControllerAddedCallback)(int controllerIndex);
typedef void(*JaegerNet_ControllerRemovedCallback)(int controllerIndex);

void JAEGERNET_EXPORT JaegerNet_StartClient(const char* const hostname, const char* const port);
void JAEGERNET_EXPORT JaegerNet_StopClient(void);

void JAEGERNET_EXPORT JaegerNet_StartInputListener(JaegerNet_ControllerAddedCallback controllerAddedCallback, JaegerNet_ControllerRemovedCallback controllerRemovedCallback);
void JAEGERNET_EXPORT JaegerNet_StopInputListener(void);

void JAEGERNET_EXPORT JaegerNet_CreateLobby(JaegerNet_ErrorCallback errorCallback, JaegerNet_LobbyCreatedCallback lobbyCreatedCallback);
void JAEGERNET_EXPORT JaegerNet_Connect(int32_t lobbyId, JaegerNet_ErrorCallback errorCallback, JaegerNet_PlayerConnectedCallback playerConnectedCallback, JaegerNet_DisconnectedCallback playerDisconnectedCallback);
void JAEGERNET_EXPORT JaegerNet_Disconnect(int controllerIndex, JaegerNet_ErrorCallback errorCallback);
