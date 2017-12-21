include_directories(../lib)

add_library(JaegerNetClient SHARED
    "Module.cpp")

target_link_libraries(JaegerNetClient
    ProtoLib
    ClientLib
    ${PROTOBUF_LIBRARY})

cotire(JaegerNetClient)
