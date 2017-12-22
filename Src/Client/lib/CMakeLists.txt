add_library(ClientLib
    "Client.cpp" "Client.h"
    "Controller.cpp" "Controller.h"
    "InputListener.cpp" "InputListener.h"
    "MessageHandler.h")

cotire(ClientLib)