#include "Module.h"
#include "Server.h"
#include "LobbyManager.h"

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    Server server(31337);

    service.run();

    return 0;
}
