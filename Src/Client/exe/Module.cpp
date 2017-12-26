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

    JaegerNet_CreateLobby([](int lobbyId, JaegerNetError error)
    {
        cout << "CreateLobbyCallback: lobbyId: " << lobbyId << ", error: " << static_cast<int32_t>(error) << endl;

        JaegerNet_Connect(lobbyId, [](int32_t playerId, int32_t playerNumber, JaegerNet::JaegerNetError error)
        {
            cout << "ConnectCallback: playerId: " << playerId << ", playerNumber: " << playerNumber << ", error: " << static_cast<int32_t>(error) << endl;
        });
    });

    getchar();

    JaegerNet_StopInputListener();
    JaegerNet_StopClient();

    return 0;
}
