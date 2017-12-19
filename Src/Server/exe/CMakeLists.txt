include_directories(../lib)

add_executable(ServerExe
    "Module.cpp" "Module.h")

target_link_libraries(ServerExe
    ProtoLib
    ServerLib
    ${PROTOBUF_LIBRARY})

cotire(ServerExe)