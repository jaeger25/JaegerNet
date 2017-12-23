#pragma once
#include <stdint.h>
#include "ErrorCodes.h"

typedef void(*CreateLobbyCallback)(int32_t lobbyId, JaegerNet::JaegerNetError error);
typedef void(*ConnectCallback)(int32_t playerId, int32_t playerNumber, JaegerNet::JaegerNetError error);
typedef void(*DisconnectCallback)(JaegerNet::JaegerNetError error);
typedef void(*ControllerAddedCallback)(int controllerIndex);
typedef void(*ControllerRemovedCallback)(int controllerIndex);

void JaegerNet_StartClient(const char* const hostname, const char* const port);
void JaegerNet_StopClient(void);

void JaegerNet_StartInputListener(::ControllerAddedCallback controllerAddedCallback, ::ControllerRemovedCallback controllerRemovedCallback);
void JaegerNet_StopInputListener(void);

void JaegerNet_CreateLobby(CreateLobbyCallback callback);
void JaegerNet_Connect(int32_t lobbyId, ConnectCallback callback);
void JaegerNet_Disconnect(int32_t playerId, DisconnectCallback callback);
