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
    bool createLobby = false;
    int32_t lobbyId = 0;

    try
    {
        TCLAP::CmdLine cmdLine("JaegerNet test utility tool");

        TCLAP::ValueArg<std::string> hostnameArg("", "hostname", "Hostname for the JaegerNet server", true, "127.0.0.1", "string");
        TCLAP::ValueArg<std::string> portArg("", "port", "Port for the JaegerNet server", true, "31337", "string");
        cmdLine.add(hostnameArg);
        cmdLine.add(portArg);

        TCLAP::ValueArg<int32_t> lobbyIdArg("", "lobbyid", "LobbyId to try and connect to", true, 0, "int");
        TCLAP::SwitchArg createLobbySwitch("", "createlobby", "Switch which indicates a new lobby should be created", false);
        cmdLine.xorAdd(lobbyIdArg, createLobbySwitch);

        cmdLine.parse(argc, argv);

        hostname = hostnameArg.getValue();
        port = portArg.getValue();

        if (lobbyIdArg.isSet())
        {
            lobbyId = lobbyIdArg.getValue();
        }
        else if (createLobbySwitch.isSet())
        {
            createLobby = true;
        }

    }
    catch (TCLAP::ArgException& ex)
    {
        cerr << "Error: " << ex.error() << " for arg [" << ex.argId() << "]" << endl;
        return 0;
    }

    JaegerNet_StartClient(hostname.c_str(), port.c_str());

    JaegerNet_StartInputListener(
        [](int controllerIndex)
    {
        cout << "Controller added: " << controllerIndex << endl;
    },
        [](int controllerIndex)
    {
        cout << "Controller removed: " << controllerIndex << endl;
    });

    auto connectErrorCallback = [](JaegerNetError error)
    {
        cout << "Connect_ErrorCallback: Error: " << static_cast<int32_t>(error) << endl;
    };

    auto playerConnectedCallback = [](int32_t playerNumber)
    {
        cout << "Connect_PlayerConnected: " << playerNumber << endl;
    };

    auto playerDisconnectedCallback = [](int32_t playerNumber)
    {
        cout << "Connect_PlayerDisconnected: " << playerNumber << endl;
    };

    if (createLobby)
    {
        JaegerNet_CreateLobby(
            [](JaegerNetError error)
        {
            cout << "CreateLobby_ErrorCallback: Error: " << static_cast<int32_t>(error) << endl;
        },
            [connectErrorCallback, playerConnectedCallback, playerDisconnectedCallback](int32_t lobbyId)
        {
            cout << "CreateLobby_Callback: LobbyId: " << lobbyId << endl;

            JaegerNet_Connect(lobbyId, connectErrorCallback, playerConnectedCallback, playerDisconnectedCallback);
        });
    }
    else
    {
        JaegerNet_Connect(lobbyId, connectErrorCallback, playerConnectedCallback, playerDisconnectedCallback);
    }

    getchar();

    JaegerNet_StopInputListener();
    JaegerNet_StopClient();

    return 0;
}
