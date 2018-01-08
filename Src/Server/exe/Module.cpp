#include "JaegerNetServer.h"

using namespace JaegerNet;

int main(int /*argc*/, char** /*argv*/)
{
    JaegerNet_StartServer(31337);

    getchar();
    return 0;
}
