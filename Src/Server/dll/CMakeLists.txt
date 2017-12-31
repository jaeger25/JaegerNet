include_directories(../lib)
include_directories("${CMAKE_BINARY_DIR}/exports")

include(GenerateExportHeader)

add_library(JaegerNetServer SHARED
    "Module.cpp")

target_link_libraries(JaegerNetServer
    PRIVATE ProtoLib
    PRIVATE ServerLib
    ${PROTOBUF_LIBRARY})

generate_export_header(JaegerNetServer
    BASE_NAME JaegerNetServer
    EXPORT_MACRO_NAME JAEGERNET_EXPORT
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/JaegerNetServer_Exports.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

cotire(JaegerNetServer)
