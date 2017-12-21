#pragma once
#include <stdint.h>
#include "ErrorCodes.h"

typedef void(*CreateLobbyCallback)(int32_t lobbyId, JaegerNet::JaegerNetError error);
typedef void(*ConnectCallback)(int32_t playerNumber, JaegerNet::JaegerNetError error);

void JaegerNet_StartClient(const char* const hostname, const char* const port);
void JaegerNet_StopClient(void);

void JaegerNet_CreateLobby(CreateLobbyCallback callback);
void JaegerNet_Connect(int32_t lobbyId, ConnectCallback callback);
