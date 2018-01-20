include_directories(../lib)
include_directories("${CMAKE_BINARY_DIR}/exports")

include(GenerateExportHeader)

add_library(JaegerNetClient SHARED
    "Module.cpp")

target_link_libraries(JaegerNetClient
    PRIVATE ProtoLib
    PRIVATE ClientLib
    PRIVATE ${PROTOBUF_LIBRARIES}
    PRIVATE SDL2::SDL2-static
    PRIVATE spdlog::spdlog
    PRIVATE fmt::fmt-header-only)

generate_export_header(JaegerNetClient
    BASE_NAME JaegerNetClient
    EXPORT_MACRO_NAME JAEGERNET_EXPORT
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/JaegerNetClient_Exports.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

cotire(JaegerNetClient)
