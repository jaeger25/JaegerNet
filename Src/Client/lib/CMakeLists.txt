add_library(ClientLib
    "Client.cpp" "Client.h"
    "Controller.cpp" "Controller.h"
    "InputListener.cpp" "InputListener.h"
    "Lobby.cpp" "Lobby.h"
    "Player.cpp" "Player.h")

cotire(ClientLib)