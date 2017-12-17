# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(PROTOBUF_PATH "Dependencies/protobuf-3.5.0" )
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${PROTOBUF_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

project ("JaegerNet")

include(cotire)
find_package(Protobuf REQUIRED)

add_definitions (-DASIO_STANDALONE -DASIO_HAS_STD_SHARED_PTR -DASIO_HAS_STD_ADDRESSOF -DASIO_HAS_STD_ARRAY -DASIO_HAS_STD_TYPE_TRAITS -DASIO_HAS_CSTDINT)

include_directories(inc)
include_directories(Dependencies/asio-1.10.6/include)
include_directories(Dependencies/protobuf-3.5.0/include)
include_directories(${CMAKE_BINARY_DIR}/proto)

# Include sub-projects.
add_subdirectory ("Client")
add_subdirectory ("Proto")
add_subdirectory ("Server")