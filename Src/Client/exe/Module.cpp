#include "JaegerNetClient.h"
#include <iostream>

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    JaegerNet_StartClient("127.0.0.1", "31337");

    JaegerNet_StartInputListener(
        [](int controllerIndex)
    {
        cout << "Controller added: " << controllerIndex << endl;
    },
        [](int controllerIndex)
    {
        cout << "Controller removed: " << controllerIndex << endl;
    });

    JaegerNet_CreateLobby(
        [](JaegerNetError error)
    {
        cout << "CreateLobby_ErrorCallback: Error: " << static_cast<int32_t>(error) << endl;
    },
        [](int32_t lobbyId)
    {
        cout << "CreateLobby_Callback: LobbyId: " << lobbyId << endl;

        JaegerNet_Connect(lobbyId,
            [](JaegerNetError error)
        {
            cout << "Connect_ErrorCallback: Error: " << static_cast<int32_t>(error) << endl;
        },
            [](int32_t playerNumber)
        {
            cout << "Connect_PlayerConnected: " << playerNumber << endl;
        },
            [](int32_t playerNumber)
        {
            cout << "Connect_PlayerDisconnected: " << playerNumber << endl;
        });
    });

    getchar();

    JaegerNet_StopInputListener();
    JaegerNet_StopClient();

    return 0;
}
