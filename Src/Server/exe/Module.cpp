#include "Module.h"
#include "Server.h"

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    asio::io_service service;

    Server server(service, 31337);

    service.run();

    return 0;
}
