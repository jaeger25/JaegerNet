#include "JaegerNetClient.h"
#include "Logging.h"
#include <iostream>
#include <mutex>
#include <tclap/CmdLine.h>

using namespace JaegerNet;
using namespace std;

std::shared_ptr<spdlog::logger> Logger;

int main(int argc, char** argv)
{
    Logger = spdlog::stdout_color_mt("console");

    std::string hostname;
    std::string port;
    std::vector<int> controllerIndices;

    try
    {
        TCLAP::CmdLine cmdLine("JaegerNet test utility tool");

        TCLAP::ValueArg<std::string> hostnameArg("", "hostname", "Hostname for the JaegerNet server", true, "127.0.0.1", "string");
        TCLAP::ValueArg<std::string> portArg("", "port", "Port for the JaegerNet server", true, "31337", "string");
        cmdLine.add(hostnameArg);
        cmdLine.add(portArg);

        TCLAP::MultiArg<int> controllerIndexArg("", "controllerindex", "Index (starting at 0) of the controller to use for input", false, "int");
        cmdLine.add(controllerIndexArg);

        cmdLine.parse(argc, argv);

        hostname = hostnameArg.getValue();
        port = portArg.getValue();
        controllerIndices = controllerIndexArg.getValue();
    }
    catch (TCLAP::ArgException& ex)
    {
        Logger->error("Error: {0} for arg [{1}]", ex.error(), ex.argId());
        return 0;
    }

    JaegerNet_StartClient(hostname.c_str(), port.c_str());

    auto controllerAddedToken = JaegerNet_RegisterControllerAddedCallback([](int controllerIndex)
    {
        Logger->info("Controller_Added: {0}", controllerIndex);
    });

    auto controllerRemovedToken = JaegerNet_RegisterControllerRemovedCallback([](int controllerIndex)
    {
        Logger->info("Controller_Removed: {0}", controllerIndex);
    });

    auto playerConnectedToken = JaegerNet_RegisterPlayerConnectedCallback([](int32_t playerNumber)
    {
        Logger->info("Connect_PlayerConnected: {0}", playerNumber);
    });

    auto playerDisconnectedToken = JaegerNet_RegisterPlayerDisconnectedCallback([](int32_t playerNumber)
    {
        Logger->info("Connect_PlayerDisconnected: {0}", playerNumber);
    });

    for (auto&& controllerIndex : controllerIndices)
    {
        JaegerNet_Connect(controllerIndex, [](JaegerNetError error)
        {
            Logger->info("Connect_ErrorCallback: {0}", static_cast<int32_t>(error));
        });
    }

    getchar();

    JaegerNet_UnregisterControllerAddedCallback(controllerAddedToken);
    JaegerNet_UnregisterControllerRemovedCallback(controllerRemovedToken);
    JaegerNet_UnregisterPlayerConnectedCallback(playerConnectedToken);
    JaegerNet_UnregisterPlayerDisconnectedCallback(playerDisconnectedToken);

    JaegerNet_StopClient();

    return 0;
}
