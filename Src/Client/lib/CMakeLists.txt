add_library(ClientLib
    "Client.cpp" "Client.h"
    "InputListener.cpp" "InputListener.h"
    "MessageHandler.h")

cotire(ClientLib)