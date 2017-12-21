#include "JaegerNetClient.h"
#include "Client.h"

bool JaegerNet_StartClient(const char* const hostname, const char* const port) try
{
    JaegerNet::CreateClient(hostname, port, {});
    JaegerNet::GetClient()->Run(true);
    return true;
}
catch (...)
{
    return false;
}

void JaegerNet_StopClient(void) try
{
    JaegerNet::DestroyClient();
}
catch (...)
{
}