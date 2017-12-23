include_directories(../lib)

add_library(JaegerNetClient SHARED
    "Factory.cpp"
    "Module.cpp")

target_link_libraries(JaegerNetClient
    ProtoLib
    ClientLib
    SDL2::SDL2 SDL2::SDL2main
    ${PROTOBUF_LIBRARY})

cotire(JaegerNetClient)
