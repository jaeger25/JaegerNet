include_directories(../lib)

add_executable(ClientExe
    "Module.cpp" "Module.h")

target_link_libraries(ClientExe
    ProtoLib
    ClientLib
    ${PROTOBUF_LIBRARY})

cotire(ClientExe)