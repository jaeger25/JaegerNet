#pragma once
#include <stdint.h>
#include <functional>
#include "JaegerNetClient_Exports.h"
#include "ErrorCodes.h"

typedef std::function<void(JaegerNet::JaegerNetError error)> JaegerNet_ErrorCallback;
typedef std::function<void(int32_t lobbyId)> JaegerNet_LobbyCreatedCallback;
typedef std::function<void(int32_t playerNumber)> JaegerNet_PlayerConnectedCallback;
typedef std::function<void(int32_t playerNumber)> JaegerNet_DisconnectedCallback;
typedef std::function<void(int controllerIndex)> JaegerNet_ControllerAddedCallback;
typedef std::function<void(int controllerIndex)> JaegerNet_ControllerRemovedCallback;

void JAEGERNET_EXPORT JaegerNet_StartClient(const char* const hostname, const char* const port);
void JAEGERNET_EXPORT JaegerNet_StopClient(void);

void JAEGERNET_EXPORT JaegerNet_StartInputListener(JaegerNet_ControllerAddedCallback controllerAddedCallback, JaegerNet_ControllerRemovedCallback controllerRemovedCallback);
void JAEGERNET_EXPORT JaegerNet_StopInputListener(void);

void JAEGERNET_EXPORT JaegerNet_CreateLobby(JaegerNet_ErrorCallback errorCallback, JaegerNet_LobbyCreatedCallback lobbyCreatedCallback);
void JAEGERNET_EXPORT JaegerNet_Connect(int32_t lobbyId, JaegerNet_ErrorCallback errorCallback, JaegerNet_PlayerConnectedCallback playerConnectedCallback, JaegerNet_DisconnectedCallback playerDisconnectedCallback);
void JAEGERNET_EXPORT JaegerNet_Disconnect(int controllerIndex, JaegerNet_ErrorCallback errorCallback);
