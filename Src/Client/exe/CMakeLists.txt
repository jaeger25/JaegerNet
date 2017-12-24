include_directories(${CMAKE_BINARY_DIR}/exports)

add_executable(ClientExe
    "Module.cpp")

target_link_libraries(ClientExe
    ProtoLib
    JaegerNetClient
    SDL2::SDL2 SDL2::SDL2main
    ${PROTOBUF_LIBRARY})

cotire(ClientExe)