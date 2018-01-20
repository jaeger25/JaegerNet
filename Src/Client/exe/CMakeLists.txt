include_directories(${CMAKE_BINARY_DIR}/exports)

add_executable(ClientExe
    "Module.cpp")

target_link_libraries(ClientExe
    ProtoLib
    JaegerNetClient
    ${PROTOBUF_LIBRARIES}
    SDL2::SDL2-static
    spdlog::spdlog
    fmt::fmt-header-only)

cotire(ClientExe)