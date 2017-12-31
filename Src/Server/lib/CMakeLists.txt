add_library (ServerLib 
    "Server.cpp" "Server.h"
    "Lobby.cpp" "Lobby.h"
    "Player.cpp" "Player.h"
    "Session.cpp" "Session.h")

add_dependencies(ServerLib protobuf_compilation)
cotire(ServerLib)