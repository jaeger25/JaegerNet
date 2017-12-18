#include "Module.h"
#include "Server.h"

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    asio::io_service ioService;

    MessageHandler messageHandler;
    Server server(messageHandler, ioService, 31337);

    ioService.run();

    return 0;
}
