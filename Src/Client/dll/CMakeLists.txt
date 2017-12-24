include_directories(../lib)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include(GenerateExportHeader)

add_library(JaegerNetClient SHARED
    "Factory.cpp"
    "Module.cpp")

target_link_libraries(JaegerNetClient
    PRIVATE ProtoLib
    PRIVATE ClientLib
    SDL2::SDL2 SDL2::SDL2main
    ${PROTOBUF_LIBRARY})

generate_export_header(JaegerNetClient
    BASE_NAME JaegerNetClient
    EXPORT_MACRO_NAME JAEGERNET_EXPORT
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/JaegerNetClient_Exports.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

cotire(JaegerNetClient)
