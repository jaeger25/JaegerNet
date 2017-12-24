include_directories(../lib)

include(GenerateExportHeader)

add_library(JaegerNetClient SHARED
    "Factory.cpp"
    "Module.cpp")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")
GENERATE_EXPORT_HEADER(JaegerNetClient
    BASE_NAME JaegerNetClient
    EXPORT_MACRO_NAME JAEGERNET_EXPORT
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/JaegerNetClient_Exports.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

target_link_libraries(JaegerNetClient
    PRIVATE ProtoLib
    PRIVATE ClientLib
    SDL2::SDL2 SDL2::SDL2main
    ${PROTOBUF_LIBRARY})

install(TARGETS JaegerNetClient
    EXPORT JaegerNetClient
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(EXPORT JaegerNetClient DESTINATION ${CMAKE_BINARY_DIR}/exports)

cotire(JaegerNetClient)
