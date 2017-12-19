#include "Module.h"
#include "Server.h"

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    Server server(31337);
    server.Run();

    return 0;
}
