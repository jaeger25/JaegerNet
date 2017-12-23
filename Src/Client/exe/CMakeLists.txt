include_directories(../lib)

add_executable(ClientExe
    "Factory.cpp"
    "Module.cpp")

target_link_libraries(ClientExe
    ProtoLib
    ClientLib
    SDL2::SDL2 SDL2::SDL2main
    ${PROTOBUF_LIBRARY})

cotire(ClientExe)