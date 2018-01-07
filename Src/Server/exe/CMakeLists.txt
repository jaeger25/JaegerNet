include_directories(../inc)
include_directories("${CMAKE_BINARY_DIR}/exports")

add_executable(ServerExe
    "Module.cpp")

target_link_libraries(ServerExe
    JaegerNetServer)

cotire(ServerExe)
