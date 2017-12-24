#include "JaegerNetClient.h"

using namespace JaegerNet;

int main(int /*argc*/, char** /*argv*/)
{
    JaegerNet_StartClient("127.0.0.1", "31337");

    JaegerNet_StartInputListener(
        [](int)
    {
    },
        [](int)
    {
    });

    JaegerNet_StopInputListener();
    JaegerNet_StopClient();

    return 0;
}
