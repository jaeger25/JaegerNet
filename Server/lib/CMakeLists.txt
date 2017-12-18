add_library (ServerLib 
    "Server.cpp" "Server.h"
    "MessageHandler.cpp" "MessageHandler.h")

add_dependencies(ServerLib protobuf_compilation)
cotire(ServerLib)