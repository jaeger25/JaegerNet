#include "MessageHandler.h"

using namespace JaegerNet;
using namespace std;

void MessageHandler::OnMessageReceived(const JaegerNetMessage& message)
{
    if (message.has_connect())
    {
        ConnectMessage connectMessage = message.connect();
    }
}
