set_source_files_properties(${PROTO_SRC} ${PROTO_HEADER} PROPERTIES GENERATED TRUE)

add_library (ServerLib 
    "Server.cpp" "Server.h"
    "MessageHandler.cpp" "MessageHandler.h")

add_dependencies(ServerLib protobuf_compilation)
cotire(ServerLib)