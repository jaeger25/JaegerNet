#include "JaegerNetClient.h"
#include <iostream>
#include <mutex>
#include <tclap/CmdLine.h>

using namespace JaegerNet;
using namespace std;

int main(int argc, char** argv)
{
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
        cerr << "Error: " << ex.error() << " for arg [" << ex.argId() << "]" << endl;
        return 0;
    }

    JaegerNet_StartClient(hostname.c_str(), port.c_str());

    auto controllerAddedToken = JaegerNet_RegisterControllerAddedCallback([](int controllerIndex)
    {
        cout << "Controller added: " << controllerIndex << endl;
    });

    auto controllerRemovedToken = JaegerNet_RegisterControllerRemovedCallback([](int controllerIndex)
    {
        cout << "Controller removed: " << controllerIndex << endl;
    });

    auto playerConnectedToken = JaegerNet_RegisterPlayerConnectedCallback([](int32_t playerNumber)
    {
        cout << "Connect_PlayerConnected: " << playerNumber << endl;
    });

    auto playerDisconnectedToken = JaegerNet_RegisterPlayerDisconnectedCallback([](int32_t playerNumber)
    {
        cout << "Connect_PlayerDisconnected: " << playerNumber << endl;
    });

    for (auto&& controllerIndex : controllerIndices)
    {
        JaegerNet_Connect(controllerIndex, [](JaegerNetError error)
        {
            cout << "Connect_ErrorCallback: Error: " << static_cast<int32_t>(error) << endl;
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
