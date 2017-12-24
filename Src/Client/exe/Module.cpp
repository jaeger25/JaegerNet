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

    JaegerNet_StopInputListener();
    JaegerNet_StopClient();

    return 0;
}
